/* This abstract type is used to represent a disjoint union
 * It's parameter takes a tuple of the possible types that it is, ie:
 * sum((string, int));  // a disjoint union of `string` or `int`
 * sum((bool, string, array(int)));
 */
type sum('a);

type arbitrary('a);

module Types = {
  type proxy('a) =
    | Proxy;

  type either('a, 'b) =
    | Left('a)
    | Right('b);
  let either: ('a => 'c, 'b => 'c, either('a, 'b)) => 'c =
    (left_fn, right_fn, e) =>
      switch (e) {
      | Left(l) => left_fn(l)
      | Right(r) => right_fn(r)
      };
  let left: 'a => either('a, 'b) = l => Left(l);
  let right: 'b => either('a, 'b) = r => Right(r);
  let bimap: ('a => 'c, 'b => 'd, either('a, 'b)) => either('b, 'd) =
    (left_fn, right_fn, e) =>
      switch (e) {
      | Left(l) => Left(left_fn(l))
      | Right(r) => Right(right_fn(r))
      };
};

[@bs.module "fast-check"]
external sample: (~arb: arbitrary('a), ~genSize: int=?, unit) => array('a) =
  "sample";

module Combinators = {
  // methods on fast-check itself
  [@bs.module "fast-check"]
  external array: arbitrary('a) => arbitrary(array('a)) = "array";
  [@bs.module "fast-check"]
  external constant: 'a => arbitrary('a) = "constant";
  [@bs.module "fast-check"] [@bs.variadic]
  external constantFrom: array('a) => arbitrary('a) = "constantFrom";
  [@bs.module "fast-check"] [@bs.variadic]
  external oneOf: array(arbitrary('a)) => arbitrary('a) = "oneOf";


  // methods on the arbitraries
  [@bs.send]
  external chain: (arbitrary('a), 'a => arbitrary('b)) => arbitrary('b) =
    "chain";
  [@bs.send]
  external map: (arbitrary('a), 'a => 'b) => arbitrary('b) = "map";
};

[@bs.module "fast-check"]
external boolean: unit => arbitrary(bool) = "boolean";

[@bs.module "fast-check"]
external integer: unit => arbitrary(int) = "integer";
[@bs.module "fast-check"]
external integerRange: (int, int) => arbitrary(int) = "integer";
[@bs.module "fast-check"]
external nat: (~max: int=?, unit) => arbitrary(int) = "nat";
[@bs.module "fast-check"]
external maxSafeInteger: unit => arbitrary(int) = "maxSafeInteger";
[@bs.module "fast-check"]
external maxSafeNat: unit => arbitrary(int) = "maxSafeNat";

// `float` would shadow the pervasives module. What should we call it?
// [@bs.module "fast-check"] external float: unit => arbitrary(float) = "float";
[@bs.module "fast-check"]
external floatRange: (float, float) => arbitrary(float) = "float";

[@bs.module "fast-check"]
external double: unit => arbitrary(float) = "double";
[@bs.module "fast-check"]
external doubleRange: (float, float) => arbitrary(float) = "double";

[@bs.module "fast-check"] external char: unit => arbitrary(string) = "char";
[@bs.module "fast-check"]
external char16bits: unit => arbitrary(string) = "char16bits";
[@bs.module "fast-check"] external hexa: unit => arbitrary(string) = "hexa";
[@bs.module "fast-check"]
external base64: unit => arbitrary(string) = "base64";
[@bs.module "fast-check"] external ascii: unit => arbitrary(string) = "ascii";
[@bs.module "fast-check"]
external fullUnicode: unit => arbitrary(string) = "fullUnicode";

[@bs.module "fast-check"]
external hexaString: unit => arbitrary(string) = "hexaString";
[@bs.module "fast-check"]
external hexaStringRange: (int, int) => arbitrary(string) = "hexaString";
[@bs.module "fast-check"]
external base64String: unit => arbitrary(string) = "base64String";
[@bs.module "fast-check"]
external base64StringRange: (int, int) => arbitrary(string) = "base64String";
[@bs.module "fast-check"]
external string: unit => arbitrary(string) = "string";
[@bs.module "fast-check"]
external stringRange: (int, int) => arbitrary(string) = "string";
[@bs.module "fast-check"]
external asciiString: unit => arbitrary(string) = "asciiString";
[@bs.module "fast-check"]
external asciiStringRange: (int, int) => arbitrary(string) = "asciiString";
[@bs.module "fast-check"]
external string16Bits: unit => arbitrary(string) = "string16Bits";
[@bs.module "fast-check"]
external string16BitsRange: (int, int) => arbitrary(string) = "string16Bits";
[@bs.module "fast-check"]
external fullUnicodeString: unit => arbitrary(string) = "fullUnicodeString";
[@bs.module "fast-check"]
external fullUnicodeStringRange: (int, int) => arbitrary(string) =
  "fullUnicodeString";
[@bs.module "fast-check"]
external stringOf: arbitrary(string) => arbitrary(string) = "stringOf";
[@bs.module "fast-check"]
external stringOfRange: (arbitrary(string), int, int) => arbitrary(string) =
  "stringOf";

[@bs.module "fast-check"]
external date: unit => arbitrary(Js.Date.t) = "date";
[@bs.module "fast-check"]
external dateRange:
  {
    .
    "min": Js.Date.t,
    "max": Js.Date.t,
  } =>
  arbitrary(Js.Date.t) =
  "date";

let arbList: arbitrary('a) => arbitrary(list('a)) =
  a => Combinators.(map(array(a), Array.to_list));
// let arbList: arbitrary('a) => arbitrary(list('a)) =
//   a =>
//     chain(
//       Array.to_list,
//       Array.of_list,
//       ~newShow=
//         l =>
//           Js.Json.stringifyAny(Array.of_list(l))
//           |> Js.Option.getWithDefault(""),
//       array(a),
//     );

/* * * * * * * *
 * For objects *
 * * * * * * * */
[@bs.module "jsverify"] external arbObject: arbitrary(Js.t('a)) = "object";

[@bs.module "jsverify"] external arbJSON: arbitrary(Js.Json.t) = "json";

[@bs.module "jsverify"]
external arbDict: arbitrary('a) => arbitrary(Js.Dict.t('a)) = "dict";

/* * * * * *
 * Helpers *
 * * * * * */
[@bs.module "fast-check"]
external arbConstant: 'a => arbitrary('a) = "constant";

[@bs.send] external noShrink: arbitrary('a) => arbitrary('a) = "noShrink";

/*
 /* * * * * * * *
  * Combinators *
  * * * * * * * */
 // [@bs.module "jsverify"]
 // external small: arbitrary('a) => arbitrary('a) = "small";

 // [@bs.module "jsverify"]
 // external such_that: (arbitrary('a), 'a => bool) => arbitrary('a) =
 //   "suchthat";

 // [@bs.module "jsverify"]
 // external elements: array('a) => arbitrary('a) = "elements";

 // [@bs.module "jsverify"]
 // external one_of: array(arbitrary('a)) => arbitrary('a) = "oneof";

 // [@bs.module "jsverify"]
 // external fn: arbitrary('a) => arbitrary('b => 'a) = "asciinestring";

 // [@bs.module "jsverify"]
 // external tuple: ((arbitrary('a), arbitrary('b))) => arbitrary(('a, 'b)) =
 //   "tuple";

 // [@bs.module "jsverify"]
 // external tuple':
 //   ((arbitrary('a), arbitrary('b), arbitrary('c))) =>
 //   arbitrary(('a, 'b, 'c)) =
 //   "tuple";

 // [@bs.module "jsverify"]
 // external tuple'':
 //   ((arbitrary('a), arbitrary('b), arbitrary('c), arbitrary('d))) =>
 //   arbitrary(('a, 'b, 'c, 'd)) =
 //   "tuple";

 // [@bs.module "jsverify"]
 // external unsafe_record:
 //   (
 //     [@bs.ignore]
 //     Types.proxy(Js.t('a)) /* set the record type as key: value */,
 //     Js.t('b)
 //   ) => /* set the record type as key: arbitrary(value) */
 //   arbitrary(Js.t('a)) =
 //   "record";

 /* Combines several arbitraries (as an untagged union)
  * This is represented as an abstract type `sum('a)`. You'll need to use reflection in
  * order to get the value out (see the `Js.Types` module)
  */
 // [@bs.module "jsverify"]
 // external sum:
 //   ((arbitrary('a), arbitrary('b))) => arbitrary(sum(('a, 'b))) =
 //   "oneof";
 // [@bs.module "jsverify"]
 // external sum':
 //   ((arbitrary('a), arbitrary('b), arbitrary('c))) =>
 //   arbitrary(sum(('a, 'b, 'c))) =
 //   "oneof";
 // [@bs.module "jsverify"]
 // external sum'':
 //   ((arbitrary('a), arbitrary('b), arbitrary('c), arbitrary('d))) =>
 //   arbitrary(sum(('a, 'b, 'c, 'd))) =
 //   "oneof";

 // let null: arbitrary('a) => arbitrary(Js.null('a)) =
 //   arb => {
 //     let null: Js.null('a) = Js.null;
 //     Obj.magic(sum((arb, arbConstant(null))));
 //   };

 // let nullable: arbitrary('a) => arbitrary(Js.nullable('a)) =
 //   arb => {
 //     let null: Js.nullable('a) = Js.Nullable.null;
 //     let undefined: Js.nullable('a) = Js.Nullable.undefined;
 //     Obj.magic(sum'((arb, arbConstant(null), arbConstant(undefined))));
 //   };

 // let option: arbitrary('a) => arbitrary(option('a)) =
 //   arb =>
 //     smap(
 //       Js.Null.toOption,
 //       Js.Null.fromOption,
 //       ~newShow=
 //         a =>
 //           switch (a) {
 //           | Some(a') =>
 //             "Some("
 //             ++ (Js.Json.stringifyAny(a') |> Js.Option.getWithDefault(""))
 //             ++ ")"
 //           | None => "None"
 //           },
 //       null(arb),
 //     );

 // let either:
 //   (arbitrary('a), arbitrary('b)) => arbitrary(Types.either('a, 'b)) =
 //   (a, b) =>
 //     smap(
 //       r => {
 //         let is_left: sum(({. "left": 'a}, {. "right": 'b})) => bool = [%bs.raw
 //           {|
 //             function(r) { return r.left !== undefined ? 1 : 0 }
 //           |}
 //         ];
 //         is_left(r)
 //           ? Types.Left(Obj.magic(r)##left)
 //           : Types.Right(Obj.magic(r)##right);
 //       },
 //       e =>
 //         switch (e) {
 //         | Types.Left(l) => Obj.magic({"left": l})
 //         | Types.Right(r) => Obj.magic({"right": r})
 //         },
 //       ~newShow=
 //         e =>
 //           switch (e) {
 //           | Left(l') =>
 //             "Left("
 //             ++ (Js.Json.stringifyAny(l') |> Js.Option.getWithDefault(""))
 //             ++ ")"
 //           | Right(r') =>
 //             "Right("
 //             ++ (Js.Json.stringifyAny(r') |> Js.Option.getWithDefault(""))
 //             ++ ")"
 */