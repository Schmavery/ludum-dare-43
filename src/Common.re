open Reprocessing;

module AssetMap = Map.Make(String);

type puzzleSize = {
  puzzleW: int,
  puzzleH: int,
  prevPuzzleW: int,
  prevPuzzleH: int,
};

/* The main character is 0 and always appears on the left */
type dialog = {
  text: string,
  person: int
}

type piece = {
  id: int,
  kind: string,
  standX: int,
  standY: int,
  x: int,
  y: int,
  leaveText: list(dialog),
  color: colorT,
  tiles: list((int, int)),
};

type gameState =
  | Intro(int)
  | End(bool)
  | Brain
  | BrainZoom(bool, float)
  | Party
  | FadeScreen(float)
  | Conversation(list(list((dialog, string))), float);

type state = {
  /* TODO: blocked squares */
  pieces: list(piece),
  holding: option((piece, list((int, int)), int, int)),
  assets: AssetMap.t(imageT),
  sounds: AssetMap.t(soundT),
  /* Need this to tell when mouse is released */
  mousePressed: bool,
  age: int,
  gameTime: float,
  gameState,
};


let numChars = 10;
