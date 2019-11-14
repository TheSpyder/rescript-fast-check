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

module Combinators = {
  // methods on the arbitrary instances
  [@bs.send]
  external chain: (arbitrary('a), 'a => arbitrary('b)) => arbitrary('b) =
    "chain";
  [@bs.send]
  external map: (arbitrary('a), 'a => 'b) => arbitrary('b) = "map";
  [@bs.send]
  external filter: (arbitrary('a), 'a => bool) => arbitrary('a) = "filter";
  [@bs.send] external noShrink: arbitrary('a) => arbitrary('a) = "noShrink";
  [@bs.send] external noBias: arbitrary('a) => arbitrary('a) = "noBias";

  // methods on fast-check itself
  [@bs.module "fast-check"]
  external constant: 'a => arbitrary('a) = "constant";
  [@bs.module "fast-check"] [@bs.variadic]
  external constantFrom: array('a) => arbitrary('a) = "constantFrom";
  // fc.clonedConstant - unsure about this - what's fc.cloneMethod?
  [@bs.module "fast-check"] [@bs.variadic]
  external mapToConstant:
    array({
      ..
      "num": int,
      "build": int => 'a,
    }) =>
    arbitrary('a) =
    "mapToConstant";
  [@bs.module "fast-check"] [@bs.variadic]
  external oneOf: array(arbitrary('a)) => arbitrary('a) = "oneof";
  // fc.frequency - how do we distinguish WeightedArbitrary in the type system?
  // fc.option - renamed, see below
  [@bs.module "fast-check"]
  external subArray: array('a) => arbitrary(array('a)) = "subarray";
  [@bs.module "fast-check"]
  external subArrayWithLength: (array('a), int, int) => arbitrary(array('a)) =
    "subarray";
  [@bs.module "fast-check"]
  external shuffledSubArray: array('a) => arbitrary(array('a)) =
    "shuffledSubarray";
  [@bs.module "fast-check"]
  external shuffledSubArrayWithLength:
    (array('a), int, int) => arbitrary(array('a)) =
    "shuffledSubarray";
  [@bs.module "fast-check"]
  external array: arbitrary('a) => arbitrary(array('a)) = "array";
  [@bs.module "fast-check"]
  external arrayWithLength:
    (arbitrary('a), int, int) => arbitrary(array('a)) =
    "array";
  [@bs.module "fast-check"]
  external set: arbitrary('a) => arbitrary(array('a)) = "set";
  [@bs.module "fast-check"]
  external setWithLength:
    (arbitrary('a), int, int, ~comparator: ('a, 'a) => bool) =>
    arbitrary(array('a)) =
    "set";

  // slightly tweaked because tuples are arrays
  // and limited to 5 because I don't feel like doing any more
  [@bs.module "fast-check"]
  external tuple2: (arbitrary('a), arbitrary('b)) => arbitrary(('a, 'b)) =
    "tuple";
  [@bs.module "fast-check"]
  external tuple3:
    (arbitrary('a), arbitrary('b), arbitrary('c)) =>
    arbitrary(('a, 'b, 'c)) =
    "tuple";
  [@bs.module "fast-check"]
  external tuple4:
    (arbitrary('a), arbitrary('b), arbitrary('c), arbitrary('d)) =>
    arbitrary(('a, 'b, 'c, 'd)) =
    "tuple";
  [@bs.module "fast-check"]
  external tuple5:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      arbitrary('e)
    ) =>
    arbitrary(('a, 'b, 'c, 'd, 'e)) =
    "tuple";

  [@bs.module "fast-check"]
  external dictionary:
    (arbitrary(string), arbitrary('a)) => arbitrary(Js.Dict.t('a)) =
    "record";
  [@bs.module "fast-check"]
  external record:
    (Js.Dict.t(arbitrary('a)), {. "withDeletedKeys": bool}) =>
    arbitrary(Js.Dict.t('a)) =
    "record";
  // shadow for ease of use
  let record = (arb, ~withDeletedKeys) =>
    record(arb, {"withDeletedKeys": withDeletedKeys});
  /** generates "tuples" but we can't really express that in the type system */
  [@bs.module "fast-check"]
  external dedup: (arbitrary('a), int) => arbitrary(array('a)) = "dedup";

  // custom and renamed combinators
  [@bs.module "fast-check"]
  external null: arbitrary('a) => arbitrary(Js.null('a)) = "option";
  /**
   * In fast-check `option` means null. In these bindings, that has been renamed to `null`
   * and this function returns an option.
   */
  let option: arbitrary('a) => arbitrary(option('a)) =
    arb => null(arb)->map(Js.Null.toOption);

  let list: arbitrary('a) => arbitrary(list('a)) =
    a => map(array(a), Array.to_list);
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

[@bs.module "fast-check"] external float_: unit => arbitrary(float) = "float";
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
external hexaStringWithLength: (int, int) => arbitrary(string) = "hexaString";
[@bs.module "fast-check"]
external base64String: unit => arbitrary(string) = "base64String";
[@bs.module "fast-check"]
external base64StringWithLength: (int, int) => arbitrary(string) =
  "base64String";
[@bs.module "fast-check"]
external string: unit => arbitrary(string) = "string";
[@bs.module "fast-check"]
external stringWithLength: (int, int) => arbitrary(string) = "string";
[@bs.module "fast-check"]
external asciiString: unit => arbitrary(string) = "asciiString";
[@bs.module "fast-check"]
external asciiStringWithLength: (int, int) => arbitrary(string) =
  "asciiString";
[@bs.module "fast-check"]
external string16Bits: unit => arbitrary(string) = "string16bits";
[@bs.module "fast-check"]
external string16BitsWithLength: (int, int) => arbitrary(string) =
  "string16bits";
[@bs.module "fast-check"]
external fullUnicodeString: unit => arbitrary(string) = "fullUnicodeString";
[@bs.module "fast-check"]
external fullUnicodeStringWithLength: (int, int) => arbitrary(string) =
  "fullUnicodeString";
[@bs.module "fast-check"]
external stringOf: arbitrary(string) => arbitrary(string) = "stringOf";
[@bs.module "fast-check"]
external stringOfWithLength:
  (arbitrary(string), int, int) => arbitrary(string) =
  "stringOf";

// TODO More specific strings: json, lorem, etc etc

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

module Objects = {
  // There's no way to express this, so we cheat
  type any;
  /** This is totally unsafe, but objects are the one point where fast-check generates hetrogeneous types */
  external anyArb: arbitrary('a) => arbitrary(any) = "%identity";

  [@bs.deriving {abstract: light}]
  type settings = {
    [@bs.optional]
    maxDepth: int,
    [@bs.optional]
    maxKeys: int,
    [@bs.optional]
    key: arbitrary(string),
    [@bs.optional]
    values: array(arbitrary(any)),
    [@bs.optional]
    withBoxedValues: bool,
    [@bs.optional]
    withMap: bool,
    [@bs.optional]
    withSet: bool,
    [@bs.optional]
    withObjectString: bool
  };
  [@bs.module "fast-check"]
  external anything: (~settings: settings=?, unit) => arbitrary(any) = "anything";
  [@bs.module "fast-check"]
  external object_: (~settings: settings=?, unit) => arbitrary(Js.Dict.t(any)) = "object";
  [@bs.module "fast-check"]
  external jsonObject: (int) => arbitrary(Js.Dict.t(Js.Json.t)) = "jsonObject";
  [@bs.module "fast-check"]
  external unicodeJsonObject: (int) => arbitrary(Js.Dict.t(Js.Json.t)) = "unicodeJsonObject";

  [@bs.module "fast-check"]
  external letrec:
    (((. Js.Dict.key) => arbitrary(any)) => Js.Dict.t(arbitrary(any))) =>
    Js.Dict.t(arbitrary(any)) =
    "letrec";
};
