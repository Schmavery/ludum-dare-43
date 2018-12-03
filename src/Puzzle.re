open Reprocessing;
open Common;
let drawPiece = (px, py, tileSize, alpha, {color, tiles}, env) => {
  Draw.noStroke(env);
  Draw.fill({...color, a: float_of_int(alpha) /. 255.}, env);
  List.iter(
    ((x, y)) =>
      Draw.rect(
        ~pos=(px + tileSize * x, py + tileSize * y),
        ~width=tileSize,
        ~height=tileSize,
        env,
      ),
    tiles,
  );
};

let getDropTarget =
    (~mx, ~my, ~puzzleOffsetX, ~puzzleOffsetY, ~tileSize, ~holdX, ~holdY) => {
  let dropX = (mx - holdX - puzzleOffsetX + tileSize / 2) / tileSize;
  let dropY = (my - holdY - puzzleOffsetY + tileSize / 2) / tileSize;
  (dropX, dropY);
};

let rotatePoint = ((x, y)) => {
  let sin90 = 1;
  let cos90 = 0;
  let rx = x * cos90 - y * sin90;
  let ry = x * sin90 + y * cos90;
  (rx, ry);
};

let rotatePiece = piece => {
  ...piece,
  tiles: List.map(rotatePoint, piece.tiles),
};

let gridColor = Utils.color(~r=150, ~g=50, ~b=50, ~a=255);
let innerGridColor = Utils.color(~r=255, ~g=187, ~b=199, ~a=255);
let maxPuzzleH = 340;
let maxPuzzleW = 340;

let update = (pieces, holding, puzzleSize, mouseUp, env) => {
  let {puzzleW, puzzleH, prevPuzzleW, prevPuzzleH} = puzzleSize;
  let tileW = maxPuzzleW / (max(puzzleW, prevPuzzleW) + 1);
  let tileH = maxPuzzleH / (max(puzzleH, prevPuzzleH) + 1);
  let tileSize = min(tileW, tileH);
  let puzzleOffsetX = 270 - tileSize * (max(puzzleW, prevPuzzleW) + 1) / 2;
  let puzzleOffsetY = 270 - tileSize * (max(puzzleH, prevPuzzleH) + 1) / 2;

  let (mx, my) = Env.mouse(env);
  let (holding: option((piece, list((int, int)), int, int)), pieces) =
    switch (Env.mousePressed(env), holding) {
    | (false, None) => (None, pieces)
    | (true, Some((p, oldTiles, holdX, holdY))) =>
      Env.keyReleased(Space, env) ?
        {
          let (holdX, holdY) = rotatePoint((holdX, holdY));
          (Some((rotatePiece(p), oldTiles, holdX, holdY)), pieces);
        } :
        (holding, pieces)
    | (true, None) =>
      let (selected, rest) =
        List.partition(
          p =>
            List.exists(
              ((tx, ty)) => {
                let screenTX = puzzleOffsetX + (p.x + tx) * tileSize;
                let screenTY = puzzleOffsetY + (p.y + ty) * tileSize;
                mx > screenTX
                && mx < screenTX
                + tileSize
                && my > screenTY
                && my < screenTY
                + tileSize;
              },
              p.tiles,
            ),
          pieces,
        );
      switch (selected) {
      | [p] => (
          Some((
            p,
            p.tiles,
            mx - (puzzleOffsetX + p.x * tileSize) - 3,
            my - (puzzleOffsetY + p.y * tileSize) + 3,
          )),
          rest,
        )
      | _ => (None, pieces)
      };
    | (false, Some((p, oldTiles, holdX, holdY))) =>
      let (newX, newY) =
        getDropTarget(
          ~mx,
          ~my,
          ~puzzleOffsetX,
          ~puzzleOffsetY,
          ~tileSize,
          ~holdX,
          ~holdY,
        );
      let isBlocked = (x, y) =>
        List.exists(
          p =>
            List.exists(
              ((tx, ty)) => tx + p.x == x && ty + p.y == y,
              p.tiles,
            ),
          pieces,
        );
      List.exists(
        ((tx, ty)) => {
          let x = newX + tx;
          let y = newY + ty;
          x < 0
          || y < 0
          || (x > prevPuzzleW || y > prevPuzzleH)
          && (x > puzzleW || y > puzzleH)
          || isBlocked(tx + newX, ty + newY);
        },
        p.tiles,
      ) ?
        (None, [{...p, tiles: oldTiles}, ...pieces]) :
        (None, [{...p, x: newX, y: newY}, ...pieces]);
    };

  let isDone =
    mouseUp ? mx > 480 && my > 420 && mx < 480 + 120 && my < 420 + 45 : false;
  (pieces, holding, isDone);
};

let draw =
    (
      pieces,
      holding: option((piece, list((int, int)), int, int)),
      assets,
      puzzleSize,
      env,
    ) => {
  let {puzzleW, puzzleH, prevPuzzleW, prevPuzzleH} = puzzleSize;
  let tileW = maxPuzzleW / (max(puzzleW, prevPuzzleW) + 1);
  let tileH = maxPuzzleH / (max(puzzleH, prevPuzzleH) + 1);
  let tileSize = min(tileW, tileH);
  let strokeW = tileSize / 8;
  let puzzleOffsetX = 270 - tileSize * (max(puzzleW, prevPuzzleW) + 1) / 2;
  let puzzleOffsetY = 270 - tileSize * (max(puzzleH, prevPuzzleH) + 1) / 2;
  let (mx, my) = Env.mouse(env);

  let brainImg = AssetMap.find("brain", assets);
  Draw.image(brainImg, ~pos=(10, 10), ~width=700, ~height=550, env);
  Draw.strokeWeight(strokeW, env);
  for (x in 0 to max(puzzleW, prevPuzzleW)) {
    for (y in 0 to max(puzzleH, prevPuzzleH)) {
      if (x <= puzzleW && y <= puzzleH) {
        Draw.fill(innerGridColor, env);
        Draw.stroke(gridColor, env);
        Draw.rect(
          ~pos=(puzzleOffsetX + tileSize * x, puzzleOffsetY + tileSize * y),
          ~width=tileSize,
          ~height=tileSize,
          env,
        );
      } else if (x <= prevPuzzleW && y <= prevPuzzleH) {
        Draw.fill({...innerGridColor, a: 0.1}, env);
        Draw.stroke({...gridColor, a: 0.1}, env);
        Draw.rect(
          ~pos=(puzzleOffsetX + tileSize * x, puzzleOffsetY + tileSize * y),
          ~width=tileSize,
          ~height=tileSize,
          env,
        );
      };
    };
  };
  List.iter(
    p =>
      drawPiece(
        puzzleOffsetX + tileSize * p.x,
        puzzleOffsetY + tileSize * p.y,
        tileSize,
        255,
        p,
        env,
      ),
    pieces,
  );
  switch (holding) {
  | None => ()
  | Some((p, _, holdX, holdY)) =>
    let (dropX, dropY) =
      getDropTarget(
        ~mx,
        ~my,
        ~puzzleOffsetX,
        ~puzzleOffsetY,
        ~tileSize,
        ~holdX,
        ~holdY,
      );
    drawPiece(
      puzzleOffsetX + tileSize * dropX,
      puzzleOffsetY + tileSize * dropY,
      tileSize,
      100,
      p,
      env,
    );
  };

  let numCharsDrawn = ref(0);
  let charsPerCol = 4;
  let charListOffsetX = 460;
  Draw.pushStyle(env);
  for (i in 1 to numChars + 1) {
    let char =
      switch (holding, List.filter(p => p.id == i, pieces)) {
      | (_, [c]) => Some((c, false))
      | (Some((c, _, _, _)), _) when c.id == i => Some((c, true))
      | _ => None
      };
    switch (char) {
    | None => ()
    | Some((char, isHeld)) =>
      let isOutside =
        List.exists(
          ((tx, ty)) => tx + char.x > puzzleW || ty + char.y > puzzleH,
          char.tiles,
        );
      let img = AssetMap.find("face_" ++ string_of_int(i), assets);
      if (isHeld) {
        Draw.stroke(Constants.white, env);
      } else {
        Draw.noStroke(env);
      };
      if (isOutside) {
        Draw.tint({...Constants.white, a: 0.5}, env);
      } else {
        Draw.tint(Constants.white, env);
      };
      Draw.fill(char.color, env);
      Draw.ellipse(
        ~center=(
          charListOffsetX + 35 + 70 * (numCharsDrawn^ / charsPerCol),
          80 * (numCharsDrawn^ mod charsPerCol) + 35 + 90,
        ),
        ~radx=30,
        ~rady=35,
        env,
      );
      Draw.image(
        img,
        ~pos=(
          charListOffsetX + 70 * (numCharsDrawn^ / charsPerCol),
          80 * (numCharsDrawn^ mod charsPerCol) + 90,
        ),
        ~width=70,
        ~height=70,
        env,
      );
      Draw.tint(Constants.white, env);
      if (isOutside) {
        Draw.image(
          AssetMap.find("x", assets),
          ~pos=(
            charListOffsetX + 5 + 70 * (numCharsDrawn^ / charsPerCol),
            5 + 80 * (numCharsDrawn^ mod charsPerCol) + 90,
          ),
          ~width=60,
          ~height=60,
          env,
        );
      };
      numCharsDrawn := numCharsDrawn^ + 1;
    };
  };
  Draw.popStyle(env);
  Draw.image(
    AssetMap.find("continue", assets),
    ~pos=(480, 420),
    ~width=120,
    ~height=45,
    env,
  );
};
