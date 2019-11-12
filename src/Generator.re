type generator('a) = int => 'a;

[@b.send.pipe: generator('a)]
external bless: (int => 'a) => generator('a) = "bless";

[@b.send.pipe: generator('a)]
external map: ('a => 'b) => generator('b) = "map";

[@b.send.pipe: generator('a)]
external flatmap: ('a => generator('b)) => generator('b) = "flatmap";

[@b.send.pipe: generator('a)]
external constant: 'b => generator('b) = "constant";