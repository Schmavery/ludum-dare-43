open Reprocessing;
open Common;

let loadSounds = env =>
  List.fold_left(
    (acc, s) =>
      AssetMap.add(s, Env.loadSound("./assets/" ++ s ++ ".wav", env), acc),
    AssetMap.empty,
    [
      "background_music",
      "cheer",
      "kid_mumble_1",
      "kid_mumble_2",
      "kid_mumble_3",
      "lady_mumble_1",
      "lady_mumble_2",
      "lady_mumble_3",
      "man_mumble_1",
      "man_mumble_2",
      "man_mumble_3",
      "old_mumble_1",
      "old_mumble_2",
      "old_mumble_3",
    ],
  );

let loadAssets = env => {
  let map = ref(AssetMap.empty);
  map :=
    List.fold_left(
      (acc, s) =>
        AssetMap.add(
          s,
          Draw.loadImage(~filename="./assets/" ++ s ++ ".png", env),
          acc,
        ),
      map^,
      [
        "brain",
        "cake",
        "happy",
        "birthday",
        "continue",
        "space_to_continue",
        "title",
        "x",
        "instructions_1",
        "instructions_2",
        "ending_win",
        "ending_lose",
        "80",
        "81",
        "82",
        "83",
        "84",
        "85",
      ],
    );
  for (i in 0 to numChars) {
    let si = string_of_int(i);
    map :=
      AssetMap.add(
        "body_" ++ si,
        Draw.loadImage(~filename="./assets/body_" ++ si ++ ".png", env),
        map^,
      );
    map :=
      AssetMap.add(
        "face_" ++ si,
        Draw.loadImage(~filename="./assets/face_" ++ si ++ ".png", env),
        map^,
      );
  };
  map^;
};

let partitionByOutside = (pieces, puzzleW, puzzleH) =>
  List.partition(
    char =>
      !
        List.exists(
          ((tx, ty)) => tx + char.x > puzzleW || ty + char.y > puzzleH,
          char.tiles,
        ),
    pieces,
  );

let puzzleSizes = [|
  {puzzleW: 10, puzzleH: 10, prevPuzzleW: 10, prevPuzzleH: 13},
  {puzzleW: 7, puzzleH: 9, prevPuzzleW: 10, prevPuzzleH: 10},
  {puzzleW: 8, puzzleH: 6, prevPuzzleW: 7, prevPuzzleH: 9},
  {puzzleW: 6, puzzleH: 7, prevPuzzleW: 8, prevPuzzleH: 6},
  {puzzleW: 6, puzzleH: 6, prevPuzzleW: 6, prevPuzzleH: 7},
  {puzzleW: 5, puzzleH: 5, prevPuzzleW: 6, prevPuzzleH: 6},
|];

let getPuzzleSize = age => puzzleSizes[age - 80];

let drawPressSpaceToContinue = (gameTime, assets, env) => {
  Draw.pushStyle(env);
  let cycleLength = 3.0;
  let halfCycle = cycleLength /. 2.;
  let cycleTime = mod_float(gameTime, cycleLength);

  Draw.tint(
    {
      ...Constants.white,
      a:
        cycleTime < halfCycle ?
          cycleTime /. halfCycle : 1. -. (cycleTime -. halfCycle) /. halfCycle,
    },
    env,
  );
  Draw.image(
    AssetMap.find("space_to_continue", assets),
    ~pos=(545, 530),
    ~width=190,
    ~height=40,
    env,
  );
  Draw.popStyle(env);
};

let zoomTime = 0.5;
let waitTime = 0.8;
let fadeTime = 0.3;
let brainAnimTime = fadeTime +. waitTime +. zoomTime;

let setup = env => {
  Env.size(~width=750, ~height=580, env);
  let sounds = loadSounds(env);
  Env.playSound(
    AssetMap.find("background_music", sounds),
    ~volume=0.015,
    ~loop=true,
    env,
  );
  {
    pieces: Chars.startPieces,
    holding: None,
    assets: loadAssets(env),
    sounds,
    mousePressed: Env.mousePressed(env),
    age: 80,
    gameTime: 0.,
    gameState: Intro(0),
  };
};

let draw =
    (
      {
        pieces,
        holding,
        assets,
        sounds,
        mousePressed,
        age,
        gameTime,
        gameState,
      },
      env,
    ) => {
  /* TODO Remove this debug thing */
  let (gameState, pieces, assets, sounds) =
    Env.key(R, env) ?
      (Brain, Chars.startPieces, loadAssets(env), loadSounds(env)) :
      (gameState, pieces, assets, sounds);

  let (pieces, holding, gameState, age) =
    switch (gameState) {
    | End(playerWon) =>
      Draw.background(Constants.black, env);
      if (playerWon){
        Draw.image(AssetMap.find("ending_win", assets), ~pos=(180, 140), env);
      } else {
        Draw.image(AssetMap.find("ending_lose", assets), ~pos=(180, 140), env);
      }
      drawPressSpaceToContinue(gameTime, assets, env);
      (
        Chars.startPieces,
        None,
        Env.keyReleased(Space, env) ? Intro(0) : End(playerWon),
        80,
      );
    | Intro(3) =>
      Env.playSound(
        AssetMap.find("cheer", sounds),
        ~volume=0.05,
        ~loop=false,
        env,
      );
      (pieces, holding, Party, 80);
    | Intro(n) =>
      Party.draw(pieces, holding, age, assets, false, env);
      let (assetName, pos) =
        switch (n) {
        | 0 => ("title", (110, 50))
        | 1 => ("instructions_1", (110, 30))
        | _ => ("instructions_2", (110, 30))
        };
      Draw.image(AssetMap.find(assetName, assets), ~pos, env);
      drawPressSpaceToContinue(gameTime, assets, env);
      (
        pieces,
        holding,
        Env.keyReleased(Space, env) ? Intro(n + 1) : Intro(n),
        age,
      );
    | Party =>
      Party.draw(pieces, holding, age, assets, true, env);
      drawPressSpaceToContinue(gameTime, assets, env);
      (
        pieces,
        holding,
        Env.keyReleased(Space, env) ? BrainZoom(true, 0.0) : Party,
        age,
      );
    | Brain =>
      Party.draw(pieces, holding, age, assets, true, env);
      let puzzleSize = getPuzzleSize(age);
      let (pieces, holding, isDone) =
        Puzzle.update(
          pieces,
          holding,
          puzzleSize,
          mousePressed && !Env.mousePressed(env),
          env,
        );
      Puzzle.draw(pieces, holding, assets, puzzleSize, env);
      (
        pieces,
        holding,
        isDone ? BrainZoom(false, brainAnimTime) : Brain,
        age,
      );
    | BrainZoom(towardsPlayer, time) =>
      Party.draw(pieces, holding, age, assets, true, env);
      let dt = Env.deltaTime(env) *. (towardsPlayer ? 1. : (-1.));
      let maxW = 700;
      let maxH = 550;
      let minW = 50;
      let minH = 30;
      let maxX = 10;
      let maxY = 10;
      let minX = 350;
      let minY = 260;
      Draw.tint({...Constants.white, a: min(1.0, time /. fadeTime)}, env);
      let value = max(0.0, (time -. fadeTime -. waitTime) /. zoomTime);
      let brainImg = AssetMap.find("brain", assets);
      Draw.image(
        brainImg,
        ~pos=(
          Utils.lerp(~low=minX, ~high=maxX, ~value),
          Utils.lerp(~low=minY, ~high=maxY, ~value),
        ),
        ~width=Utils.lerp(~low=minW, ~high=maxW, ~value),
        ~height=Utils.lerp(~low=minH, ~high=maxH, ~value),
        env,
      );
      Draw.tint(Constants.white, env);
      if (towardsPlayer && time > brainAnimTime) {
        (pieces, holding, Brain, age);
      } else if (!towardsPlayer && time < 0.) {
        let {puzzleW, puzzleH} = getPuzzleSize(age);
        let (_inpieces, outpieces) =
          partitionByOutside(pieces, puzzleW, puzzleH);
        (
          pieces,
          holding,
          Conversation(
            List.map(
              p =>
                List.map(
                  t => (t, t.person == 0 ? "old" : p.kind),
                  p.leaveText,
                ),
              outpieces,
            ),
            0.0,
          ),
          age,
        );
      } else {
        (pieces, holding, BrainZoom(towardsPlayer, time +. dt), age);
      };
    | Conversation([], _) => (pieces, holding, FadeScreen(0.0), age)
    | Conversation([[], ...peopleRest], time) =>
      Party.draw(pieces, holding, age, assets, true, env);
      /* Pause between conversations */
      (
        pieces,
        holding,
        time > 1.5 ?
          Conversation(peopleRest, 0.) :
          Conversation([[], ...peopleRest], time +. Env.deltaTime(env)),
        age,
      );
    | Conversation(
        [[({person, text}, kind), ...dialogRest], ...peopleRest] as allDialog,
        time,
      ) =>
      if (time == 0.) {
        Env.playSound(
          AssetMap.find(
            kind ++ "_mumble_" ++ string_of_int(Random.int(3) + 1),
            sounds,
          ),
          ~volume=0.02,
          ~loop=false,
          env,
        );
      };
      Party.draw(pieces, holding, age, assets, true, env);
      let scrollTime = 0.5;
      let promptTime = 3.0;
      Draw.pushStyle(env);
      Draw.fill(Utils.color(~r=230, ~g=230, ~b=230, ~a=220), env);
      Draw.strokeWeight(2, env);
      Draw.stroke(Utils.color(~r=0, ~g=0, ~b=0, ~a=255), env);
      Draw.rect(
        ~pos=(20, 20),
        ~width=Env.width(env) - 40,
        ~height=150,
        env,
      );
      let face = AssetMap.find("face_" ++ string_of_int(person), assets);
      let textlen = String.length(text);
      let text =
        String.sub(
          text,
          0,
          min(textlen, Utils.lerp(~low=0, ~high=textlen, ~value=time /. scrollTime)),
        );
      let lines = Utils.split(text, ~sep='|');
      if (person == 0) {
        Draw.image(face, ~pos=(30, 35), ~width=120, ~height=120, env);
        List.iteri(
          (i, body) => Draw.text(~body, ~pos=(170, 45 + i * 40), env),
          lines,
        );
      } else {
        Draw.image(face, ~pos=(605, 35), ~width=120, ~height=120, env);
        List.iteri(
          (i, body) => Draw.text(~body, ~pos=(50, 45 + i * 40), env),
          lines,
        );
      };
      if (time > promptTime) {
        drawPressSpaceToContinue(gameTime, assets, env);
      };
      Draw.popStyle(env);
      (
        pieces,
        holding,
        time > scrollTime +. 0.1 && Env.keyReleased(Space, env) ?
          Conversation([dialogRest, ...peopleRest], 0.) :
          Conversation(allDialog, time +. Env.deltaTime(env)),
        age,
      );
    | FadeScreen(time) =>
      Party.draw(pieces, holding, age, assets, true, env);
      let fadeout = 2.;
      let fadein = 0.6;
      let animTime = fadeout +. fadein;
      let dt = Env.deltaTime(env);
      Draw.pushStyle(env);
      Draw.fill(
        {
          ...Constants.black,
          a:
            time < fadeout ?
              min(1.0, time /. fadeout) :
              1. -. min(1.0, (time -. fadeout) /. fadein),
        },
        env,
      );
      Draw.rect(
        ~pos=(0, 0),
        ~height=Env.height(env),
        ~width=Env.width(env),
        env,
      );
      Draw.popStyle(env);
      let (age, pieces) =
        if (time < fadeout && time +. dt >= fadeout) {
          let {puzzleW, puzzleH} = getPuzzleSize(age);
          let (inpieces, _outpieces) =
            partitionByOutside(pieces, puzzleW, puzzleH);
          (age + 1, inpieces);
        } else {
          (age, pieces);
        };
      if (time > animTime) {
        Env.playSound(
          AssetMap.find("cheer", sounds),
          ~volume=0.05,
          ~loop=false,
          env,
        );
      };
      let nextState = if (pieces == []) {
        End(false)
      } else if (age > 85) {
        End(true)
      } else if (time > animTime) {
        Party
      } else {
        FadeScreen(time +. dt)
      };
      (
        pieces,
        holding,
        nextState,
        age,
      );
    };
  {
    pieces,
    holding,
    assets,
    sounds,
    mousePressed: Env.mousePressed(env),
    age,
    gameTime: gameTime +. Env.deltaTime(env),
    gameState,
  };
};

run(~setup, ~draw, ());
