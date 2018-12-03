open Reprocessing;
open Common;

let draw = (pieces, holding, age, assets, balloons, env) => {
  Draw.background(Utils.color(~r=240, ~g=220, ~b=200, ~a=255), env);
  Draw.fill(Utils.color(~r=180, ~g=150, ~b=115, ~a=255), env);
  Draw.noStroke(env);
  Draw.rect(~pos=(0, 400), ~width=Env.width(env), ~height=400, env);
  if (balloons) {
    Draw.image(
      AssetMap.find("happy", assets),
      ~pos=(80, 90),
      ~width=300,
      ~height=120,
      env,
    );
    Draw.image(
      AssetMap.find("birthday", assets),
      ~pos=(330, 120),
      ~width=400,
      ~height=120,
      env,
    );
  };
  let drawnPeople =
    switch (holding) {
    | None => pieces
    | Some((p, _, _, _)) => [p, ...pieces]
    };
  List.iter(
    char =>
      Draw.image(
        AssetMap.find("body_" ++ string_of_int(char.id), assets),
        ~pos=(char.standX, char.standY + 40),
        ~width=140,
        ~height=240,
        env,
      ),
    List.sort((p1, p2) => compare(p1.standY, p2.standY), drawnPeople),
  );
  Draw.image(
    AssetMap.find("body_0", assets),
    ~pos=(305, 180),
    ~width=140,
    ~height=240,
    env,
  );
  Draw.image(
    AssetMap.find("cake", assets),
    ~pos=(300, 350),
    ~width=150,
    ~height=150,
    env,
  );
  Draw.image(
    AssetMap.find(string_of_int(age), assets),
    ~pos=(365, 337),
    ~width=25,
    ~height=18,
    env,
  );
};
