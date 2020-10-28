type generator('a) = int => 'a;

[@bs.send.pipe: generator('a)] external bless: (int => 'a) => generator('a) = "bless";

[@bs.send.pipe: generator('a)] external map: ('a => 'b) => generator('b) = "map";

[@bs.send.pipe: generator('a)]
external flatmap: ('a => generator('b)) => generator('b) = "flatmap";

[@bs.send.pipe: generator('a)] external constant: 'b => generator('b) = "constant";
