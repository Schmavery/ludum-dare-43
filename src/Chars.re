open Reprocessing;
open Common;

let boyLeave =  [
  {person: 1, text: "Happy birthday great grandpa Joe!"},
  {person: 0, text: "Thanks, son!|Say, how old are you now?"},
  {person: 1, text: "Six and a half! I'm in the first grade!"},
  {person: 0, text: "Wow! Do they still teach cursive,|Tommy?"},
  {person: 1, text: "My name's not Tommy..."},
  {person: 0, text: "..."},
  {person: 0, text: "I'm sorry, son.|Remind me your name?"},
  {person: 1, text: "I'm Joe - Joseph Murphy the 4th,|great grandpa."},
  {person: 1, text: "I'm named after you..."},
];

let businessLeave = [
  {person: 2, text: "How're you doing, Joe?"},
  {person: 0, text: "Just fine, Theresa, just fine."},
  {person: 2, text: "...are you sure?"},
  {person: 0, text: "Absolutely! This is going to be|a great year; I just know it."},
  {person: 2, text: "Joe, Theresa was my mother.|She passed away nearly 10 years ago."},
  {person: 0, text: "..."},
  {person: 0, text: "I'm terribly sorry.|I guess I must've forgotten."},
];

let beardLeave = [
  {person: 3, text: "Happy birthday, dad!"},
  {person: 0, text: "Mitchell, so glad you could make it!"},
  {person: 3, text: "...Dad, it's Joe."},
  {person: 3, text: "Mitch couldn't get the time off work|to fly out this year."},
  {person: 3, text: "He sends his best wishes."},
  {person: 0, text: "Oh. Your brother always was such|a hard worker."},
];

let grandpaLeave = [
  {person: 4, text: "Joe! Another year, another cake!"},
  {person: 0, text: "Glad you could make it."},
  {person: 4, text: "Feels like just yesterday we were|starting our first teaching gigs at|Lincoln High."},
  {person: 0, text: "You taught there, too?"},
  {person: 4, text: "Of course! Our classrooms were across|the hall from each other for 20 years."},
  {person: 0, text: "Right..."},
];


let nurseLeave = [
  {person: 5, text: "Sir, before you eat that cake,|you'll need to take this tablet."},
  {person: 0, text: "Why would I need that?"},
  {person: 5, text: "Sir, you've been lactose intolerant|for years."},
  {person: 0, text: "Nonsense, I've been drinking milk|all my life!"},
  {person: 0, text: "Look, whoever you are,|I'm just fine eating this cake."},
  {person: 5, text: "...Joe, I've been your nurse for|four years."},
];

let redLeave = [
  {person: 6, text: "I hope you've got enough left in your|retirement to get you through another|ten years,"},
  {person: 6, text: "the doctor seems to think you've got|another decade of birthdays ahead|of you!"},
  {person: 0, text: "Is that right? Well you can tell the|doctor that I've really appreciated|the care here."},
  {person: 0, text: "If they keep hiring great staff like|yourself, I'll gladly stick around|another ten years."},
  {person: 6, text: "Dad?"},
  {person: 0, text: "..."},
];

let braidLeave = [
  {person: 7, text: "Grandpa!"},
  {person: 0, text: "Amelia, you just don't stop growing!"},
  {person: 7, text: "Grandpa, Amelia is Uncle Joe's|daughter. I'm Mackenzie."},
  {person: 0, text: "I know that! Can't an old man make a|joke or two on his birthday?"},
  {person: 7, text: "..."},
];

let girlLeave = [
  {person: 8, text: "Can I have another piece of cake,|please?"},
  {person: 0, text: "Hello, cutie! And who might you be?"},
  {person: 8, text: "..."},
  {person: 0, text: "Don't be shy!"},
  {person: 8, text: "Grandpa, silly, stop playing games!"},
  {person: 8, text: "Mom said I could have another piece|of cake but that I had to ask|you first."},
  {person: 0, text: "..."},
];

let grandmaLeave = [
  {person: 9, text: "Joe, you old fart! I'm not going to go|easy on you tomorrow just because it's|your birthday."},
  {person: 0, text: "What's tomorrow?"},
  {person: 9, text: "Biweekly bridge night!"},
  {person: 0, text: "Of course! We've played before?"},
  {person: 9, text: "Only every other week since I moved|into this place eight years ago!"},
  {person: 0, text: "Oh..."},
];

let necklaceLeave = [
  {person: 10, text: "Happy birthday, old man!"},
  {person: 0, text: "Thanks, honey. It's such a treat when|our children come and visit us,|isn't it?"},
  {person: 10, text: "Are you feeling alright?"},
  {person: 0, text: "Of course, sweetheart."},
  {person: 10, text: "Dad, I think you might've just|confused me with mom."},
];


let startPieces = [
  /*lightning boy*/
  {
    id: 1,
    kind: "kid",
    standX: 190,
    standY: 250,
    x: 1,
    y: 12,
    leaveText: boyLeave,
    color: Utils.color(~r=255, ~g=100, ~b=100, ~a=255),
    tiles: [((-1), 0), (0, 0), (1, 0), (0, 1), (2, 0), (2, -1)],
  },
  /*businesswoman*/
  {
    id: 2,
    kind: "lady",
    standX: 130,
    standY: 200,
    x: 7,
    y: 1,
    leaveText: businessLeave,
    color: Utils.color(~r=100, ~g=100, ~b=240, ~a=255),
    tiles: [((-1), 0), (0, 0), (1, 0), (1, 1), (2, 0), (1, 2)],
  },
  /*beard*/
  {
    id: 3,
    kind: "man",
    standX: 220,
    standY: 190,
    x: 5,
    y: 5,
    leaveText: beardLeave,
    color: Utils.color(~r=210, ~g=60, ~b=10, ~a=255),
    tiles: [((-1), 0), (0, 0), (1, 0), (1, 1), (0, -1), (1, -1)],
  },
  /*grandpa*/
  {
    id: 4,
    kind: "old",
    standX: 80,
    standY: 230,
    x: 1,
    y: 6,
    leaveText: grandpaLeave,
    color: Utils.color(~r=181, ~g=225, ~b=247, ~a=255),
    tiles: [((-1), 0), (0, 0), (1, 0), ((-1), 1), (2, 0), (2, 1)],
  },
  /* nurse */
  {
    id: 5,
    kind: "man",
    standX: 40,
    standY: 170,
    x: 2,
    y: 0,
    leaveText: nurseLeave,
    color: Utils.color(~r=240, ~g=240, ~b=120, ~a=255),
    tiles: [((-1), 0), (0, 0), (0, 1), (1, 1), (1, 2), (1, 3), (0, 3)],
  },
  /*red hair*/
  {
    id: 6,
    kind: "man",
    standX: 500,
    standY: 200,
    x: 1,
    y: 4,
    leaveText: redLeave,
    /* color: Utils.color(~r=90, ~g=150, ~b=90, ~a=255), */
    color: Utils.color(~r=10, ~g=100, ~b=60, ~a=255),
    tiles: [((-1), 0), (0, 0), (1, 0), (2, 0), ((-1), 1), (0, 1)],
  },
  /* purple braid */
  {
    id: 7,
    kind: "kid",
    standX: 590,
    standY: 220,
    x: 9,
    y: 4,
    leaveText: braidLeave,
    color: Utils.color(~r=200, ~g=0, ~b=200, ~a=255),
    tiles: [(-1, 1), (0, 1), (0, 0), (1, 0), (1, -1)],
  },
  /*pigtails*/
  {
    id: 8,
    kind: "kid",
    standX: 540,
    standY: 230,
    x: 7,
    y: 11,
    leaveText: girlLeave,
    color: Utils.color(~r=100, ~g=150, ~b=200, ~a=255),
    /* color: Utils.color(~r=0, ~g=130, ~b=130, ~a=255), */
    tiles: [(-1, 0), (0, 0), (1, 0), (2, 0), (2, 1), (2, 2), (1, 2)],
  },
  /* grandma */
  {
    id: 9,
    kind: "lady",
    standX: 450,
    standY: 250,
    x: 3,
    y: 9,
    leaveText: grandmaLeave,
    color: Utils.color(~r=100, ~g=200, ~b=100, ~a=255),
    tiles: [(0, 0), (0, 1), (0, -1), (-1, -1), (1, 1), (1, -1), (-1, 1)],
  },
  /*necklace*/
  {
    id: 10,
    kind: "lady",
    standX: 400,
    standY: 200,
    x: 7,
    y: 8,
    leaveText: necklaceLeave,
    /* color: Utils.color(~r=255, ~g=166, ~b=66, ~a=255), */
    color: Utils.color(~r=255, ~g=166, ~b=0, ~a=255),
    tiles: [(-1, 0), (0, 0), (0, 1), (0, 2), (1, 0), (2, 0)],
  },
];
