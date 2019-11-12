type arbitrary('a) = Arbitrary.arbitrary('a);

type property('a);
type asyncProperty('a);

// using types to hide the fact that the same object is returned in all cases
type fcResult('a);
type result('a) =
  | Passed
  | Failed(
      {
        .
        "counterexample": 'a,
        "counterexamplePath": string,
        "error": string,
        // "executionSummary" tbd this looks hard
        "numRuns": int,
        "numShrinks": int,
        "numSkips": int,
        "seed": int,
        // "verbose" doesn't have a mapping yet
      },
    );

[@bs.get] external hasFailed: fcResult('a) => bool = "failed";

let toResult = r => hasFailed(r) ? Failed(Obj.magic(r)) : Passed;

module Parameters = {
  [@bs.deriving {abstract: light}]
  type t('a) = {
    [@bs.optional]
    endOnFailure: string,
    [@bs.optional]
    examples: array('a),
    [@bs.optional]
    interruptAfterTimeLimit: int,
    [@bs.optional]
    markInterruptAsFailure: bool,
    [@bs.optional]
    maxSkipsPerRun: int,
    [@bs.optional]
    numRuns: int,
    [@bs.optional]
    path: string,
    // [@bs.optional] randomType tbd this looks hard
    [@bs.optional]
    seed: int,
    [@bs.optional]
    skipAllAfterTimeLimit: int,
    [@bs.optional]
    timeout: int,
    [@bs.optional]
    unbiased: int,
    [@bs.optional]
    verbose: bool // todo this blocks VeryVerbose
  };

  // bs.deriving creates a `t` function with all properties as named arguments
  let make = t;
};

module FcCheck = {
  [@bs.module "fast-check"]
  external sync: property('a) => result('a) = "check";
  [@bs.module "fast-check"]
  external syncParams: (property('a), Parameters.t('a)) => result('a) =
    "check";

  [@bs.module "fast-check"]
  external async: asyncProperty('a) => Js.Promise.t(result('a)) = "check";
  [@bs.module "fast-check"]
  external asyncParams:
    (asyncProperty('a), Parameters.t('a)) => Js.Promise.t(result('a)) =
    "check";
};

module FcAssert = {
  [@bs.module "fast-check"] external sync: property('a) => unit = "assert";
  [@bs.module "fast-check"]
  external syncParams: (property('a), Parameters.t('a)) => unit = "assert";

  [@bs.module "fast-check"]
  external async: asyncProperty('a) => unit = "assert";
  [@bs.module "fast-check"]
  external asyncParams: (asyncProperty('a), Parameters.t('a)) => unit =
    "assert";
};

// Defining the externals 3 times is rough, but it's easier to use than twice with different function names
module type FcProperty = {
  type t('a);
  type r;

  [@bs.module "fast-check"]
  external property1: (arbitrary('a), 'a => r) => t('a) = "property";

  [@bs.module "fast-check"]
  external property2: (arbitrary('a), arbitrary('b), ('a, 'b) => r) => t('a) =
    "property";
  [@bs.module "fast-check"]
  external property3:
    (arbitrary('a), arbitrary('b), arbitrary('c), ('a, 'b, 'c) => r) =>
    t('a) =
    "property";

  [@bs.module "fast-check"]
  external property4:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      ('a, 'b, 'c, 'd) => r
    ) =>
    t('a) =
    "property";

  [@bs.module "fast-check"]
  external property5:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      arbitrary('e),
      ('a, 'b, 'c, 'd, 'e) => r
    ) =>
    t('a) =
    "property";

  let assertProperty1: (arbitrary('a), 'a => r) => unit;
  let assertProperty2: (arbitrary('a), arbitrary('b), ('a, 'b) => r) => unit;
  let assertProperty3:
    (arbitrary('a), arbitrary('b), arbitrary('c), ('a, 'b, 'c) => r) => unit;
  let assertProperty4:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      ('a, 'b, 'c, 'd) => r
    ) =>
    unit;
  let assertProperty5:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      arbitrary('e),
      ('a, 'b, 'c, 'd, 'e) => r
    ) =>
    unit;
};

module Sync: FcProperty with type t('a) := property('a) and type r := bool = {
  [@bs.module "fast-check"]
  external property1: (arbitrary('a), 'a => bool) => property('a) =
    "property";

  [@bs.module "fast-check"]
  external property2:
    (arbitrary('a), arbitrary('b), ('a, 'b) => bool) => property('a) =
    "property";
  [@bs.module "fast-check"]
  external property3:
    (arbitrary('a), arbitrary('b), arbitrary('c), ('a, 'b, 'c) => bool) =>
    property('a) =
    "property";

  [@bs.module "fast-check"]
  external property4:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      ('a, 'b, 'c, 'd) => bool
    ) =>
    property('a) =
    "property";

  [@bs.module "fast-check"]
  external property5:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      arbitrary('e),
      ('a, 'b, 'c, 'd, 'e) => bool
    ) =>
    property('a) =
    "property";

  let assertProperty1 = (arb, f) => FcAssert.sync(property1(arb, f));
  let assertProperty2 = (arb1, arb2, f) =>
    FcAssert.sync(property2(arb1, arb2, f));
  let assertProperty3 = (arb1, arb2, arb3, f) =>
    FcAssert.sync(property3(arb1, arb2, arb3, f));
  let assertProperty4 = (arb1, arb2, arb3, arb4, f) =>
    FcAssert.sync(property4(arb1, arb2, arb3, arb4, f));
  let assertProperty5 = (arb1, arb2, arb3, arb4, arb5, f) =>
    FcAssert.sync(property5(arb1, arb2, arb3, arb4, arb5, f));
};

type asyncResult = Js.Promise.t(bool);

module Async:
  FcProperty with type t('a) := asyncProperty('a) and type r := asyncResult = {
  [@bs.module "fast-check"]
  external property1:
    (arbitrary('a), 'a => Js.Promise.t(bool)) => asyncProperty('a) =
    "property";

  [@bs.module "fast-check"]
  external property2:
    (arbitrary('a), arbitrary('b), ('a, 'b) => Js.Promise.t(bool)) =>
    asyncProperty('a) =
    "property";

  [@bs.module "fast-check"]
  external property3:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      ('a, 'b, 'c) => Js.Promise.t(bool)
    ) =>
    asyncProperty('a) =
    "property";

  [@bs.module "fast-check"]
  external property4:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      ('a, 'b, 'c, 'd) => Js.Promise.t(bool)
    ) =>
    asyncProperty('a) =
    "property";

  [@bs.module "fast-check"]
  external property5:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      arbitrary('e),
      ('a, 'b, 'c, 'd, 'e) => Js.Promise.t(bool)
    ) =>
    asyncProperty('a) =
    "property";

  let assertProperty1 = (arb, f) => FcAssert.async(property1(arb, f));
  let assertProperty2 = (arb1, arb2, f) =>
    FcAssert.async(property2(arb1, arb2, f));
  let assertProperty3 = (arb1, arb2, arb3, f) =>
    FcAssert.async(property3(arb1, arb2, arb3, f));
  let assertProperty4 = (arb1, arb2, arb3, arb4, f) =>
    FcAssert.async(property4(arb1, arb2, arb3, arb4, f));
  let assertProperty5 = (arb1, arb2, arb3, arb4, arb5, f) =>
    FcAssert.async(property5(arb1, arb2, arb3, arb4, arb5, f));
};