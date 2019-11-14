type arbitrary('a) = Arbitrary.arbitrary('a);
type property('a);
type asyncProperty('a);
type fcResult('a);
type result('a) =
  | Passed
  | Failed(
      {
        .
        "counterexample": 'a,
        "counterexamplePath": string,
        "error": string,
        "failures": array('a),
        "numRuns": int,
        "numShrinks": int,
        "numSkips": int,
        "seed": int,
      },
    );

[@bs.get] external hasFailed: fcResult('a) => bool = "failed";

let toResult: fcResult('a) => result('b);

module Parameters: {
  [@bs.deriving {abstract: light}]
  type t('a) = {
    [@bs.optional]
    endOnFailure: bool,
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
    unbiased: bool,
    [@bs.optional]
    verbose: bool // todo this blocks VeryVerbose
  };
};

// Convenience for those who don't like calling `assert_`
module FcAssert: {
  [@bs.module "fast-check"] external sync: property('a) => unit = "assert";

  [@bs.module "fast-check"]
  external async: asyncProperty('a) => unit = "assert";
};

[@bs.module "fast-check"] external pre: bool => unit = "pre";

module Sync: {
  [@bs.module "fast-check"] external assert_: property('a) => unit = "assert";
  [@bs.module "fast-check"]
  external assertParams: (property('a), Parameters.t('a)) => unit = "assert";
  [@bs.module "fast-check"]
  external check: property('a) => fcResult('a) = "check";
  [@bs.module "fast-check"]
  external checkParams: (property('a), Parameters.t('a)) => fcResult('a) =
    "check";

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

  let assertProperty1: (arbitrary('a), 'a => bool) => unit;
  let assertProperty2:
    (arbitrary('a), arbitrary('b), ('a, 'b) => bool) => unit;
  let assertProperty3:
    (arbitrary('a), arbitrary('b), arbitrary('c), ('a, 'b, 'c) => bool) =>
    unit;
  let assertProperty4:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      ('a, 'b, 'c, 'd) => bool
    ) =>
    unit;
  let assertProperty5:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      arbitrary('e),
      ('a, 'b, 'c, 'd, 'e) => bool
    ) =>
    unit;
};

module Async: {
  [@bs.module "fast-check"]
  external assert_: asyncProperty('a) => unit = "assert";
  [@bs.module "fast-check"]
  external assertParams: (asyncProperty('a), Parameters.t('a)) => unit =
    "assert";
  [@bs.module "fast-check"]
  external check: asyncProperty('a) => fcResult('a) = "check";
  [@bs.module "fast-check"]
  external checkParams:
    (asyncProperty('a), Parameters.t('a)) => fcResult('a) =
    "check";

  [@bs.module "fast-check"]
  external property1:
    (arbitrary('a), 'a => Js.Promise.t(bool)) => asyncProperty('a) =
    "asyncProperty";

  [@bs.module "fast-check"]
  external property2:
    (arbitrary('a), arbitrary('b), ('a, 'b) => Js.Promise.t(bool)) =>
    asyncProperty('a) =
    "asyncProperty";
  [@bs.module "fast-check"]
  external property3:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      ('a, 'b, 'c) => Js.Promise.t(bool)
    ) =>
    asyncProperty('a) =
    "asyncProperty";

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
    "asyncProperty";

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
    "asyncProperty";

  let assertProperty1: (arbitrary('a), 'a => Js.Promise.t(bool)) => unit;
  let assertProperty2:
    (arbitrary('a), arbitrary('b), ('a, 'b) => Js.Promise.t(bool)) => unit;
  let assertProperty3:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      ('a, 'b, 'c) => Js.Promise.t(bool)
    ) =>
    unit;
  let assertProperty4:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      ('a, 'b, 'c, 'd) => Js.Promise.t(bool)
    ) =>
    unit;
  let assertProperty5:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      arbitrary('e),
      ('a, 'b, 'c, 'd, 'e) => Js.Promise.t(bool)
    ) =>
    unit;
};

/**
 * The void/unit modules aren't simple external mappings because BuckleScript compiles unit as `0`, not `undefined`
 *
 * The fc.property() function allows falsy predicate return values to trigger the boolean path, so we need to do
 * a bunch of manual work to make sure BuckleScript returns `undefined`
 *
 * Once they are exposed `Sync` and `Async` module types can be shared with these instead of using a Wrapped interface
 */
module type FcWrappedProperty = {
  type t('a);
  type r;

  let assert_: t('a) => unit;
  let assertParams: (t('a), Parameters.t('a)) => unit;
  let check: t('a) => fcResult('a);
  let checkParams: (t('a), Parameters.t('a)) => fcResult('a);
  let property1: (arbitrary('a), 'a => r) => t('a);
  let property2: (arbitrary('a), arbitrary('b), ('a, 'b) => r) => t('a);
  let property3:
    (arbitrary('a), arbitrary('b), arbitrary('c), ('a, 'b, 'c) => r) =>
    t('a);
  let property4:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      ('a, 'b, 'c, 'd) => r
    ) =>
    t('a);
  let property5:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      arbitrary('e),
      ('a, 'b, 'c, 'd, 'e) => r
    ) =>
    t('a);

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

module SyncUnit:
  FcWrappedProperty with type t('a) := property('a) and type r := unit;

type asyncUnitResult = Js.Promise.t(unit);

module AsyncUnit:
  FcWrappedProperty with
    type t('a) := asyncProperty('a) and type r := asyncUnitResult;