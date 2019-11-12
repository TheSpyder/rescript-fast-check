type shrink('a) = 'a => array('a);

[@b.send.pipe: shrink('a)]
external bless: ('a => array('a)) => shrink('a) = "bless";

[@b.send.pipe: shrink('a)]
external smap: ('a => 'b, 'b => 'a) => shrink('b) = "smap";