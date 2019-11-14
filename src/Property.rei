type arbitrary('a) = Arbitrary.arbitrary('a);
type property('a);
type asyncProperty('a);
type fcRunDetails('a);
type runDetails('a) =
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

[@bs.get] external hasFailed: fcRunDetails('a) => bool = "failed";

let toResult: fcRunDetails('a) => runDetails('b);

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
    logger: string => unit,
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
  external async: asyncProperty('a) => Js.Promise.t(unit) = "assert";
};

[@bs.module "fast-check"] external pre: bool => unit = "pre";

module Sync: {
  [@bs.module "fast-check"] external assert_: property('a) => unit = "assert";
  [@bs.module "fast-check"]
  external assertParams: (property('a), Parameters.t('a)) => unit = "assert";
  [@bs.module "fast-check"]
  external check: property('a) => fcRunDetails('a) = "check";
  [@bs.module "fast-check"]
  external checkParams: (property('a), Parameters.t('a)) => fcRunDetails('a) =
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
  external assert_: asyncProperty('a) => Js.Promise.t(unit) = "assert";
  [@bs.module "fast-check"]
  external assertParams:
    (asyncProperty('a), Parameters.t('a)) => Js.Promise.t(unit) =
    "assert";
  [@bs.module "fast-check"]
  external check: asyncProperty('a) => Js.Promise.t(fcRunDetails('a)) = "check";
  [@bs.module "fast-check"]
  external checkParams:
    (asyncProperty('a), Parameters.t('a)) => Js.Promise.t(fcRunDetails('a)) =
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

  // Convenience combining `assert` and `property` into a single function call
  let assertProperty1:
    (arbitrary('a), 'a => Js.Promise.t(bool)) => Js.Promise.t(unit);
  let assertProperty2:
    (arbitrary('a), arbitrary('b), ('a, 'b) => Js.Promise.t(bool)) =>
    Js.Promise.t(unit);
  let assertProperty3:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      ('a, 'b, 'c) => Js.Promise.t(bool)
    ) =>
    Js.Promise.t(unit);
  let assertProperty4:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      ('a, 'b, 'c, 'd) => Js.Promise.t(bool)
    ) =>
    Js.Promise.t(unit);
  let assertProperty5:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      arbitrary('e),
      ('a, 'b, 'c, 'd, 'e) => Js.Promise.t(bool)
    ) =>
    Js.Promise.t(unit);
};

/**
 * The void/unit modules aren't simple external mappings because BuckleScript compiles unit as `0`, not `undefined`
 *
 * The fc.property() function allows falsy predicate return values to trigger the boolean path, so we need to do
 * a bunch of manual work to make sure BuckleScript returns `undefined`
 *
 * Once they are exposed `Sync` and `Async` module types can be shared with these instead of using a Wrapped interface
 */

module SyncUnit: {
  let assert_: property('a) => unit;
  let assertParams: (property('a), Parameters.t('a)) => unit;
  let check: property('a) => fcRunDetails('a);
  let checkParams: (property('a), Parameters.t('a)) => fcRunDetails('a);
  let property1: (arbitrary('a), 'a => unit) => property('a);
  let property2:
    (arbitrary('a), arbitrary('b), ('a, 'b) => unit) => property('a);
  let property3:
    (arbitrary('a), arbitrary('b), arbitrary('c), ('a, 'b, 'c) => unit) =>
    property('a);
  let property4:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      ('a, 'b, 'c, 'd) => unit
    ) =>
    property('a);
  let property5:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      arbitrary('e),
      ('a, 'b, 'c, 'd, 'e) => unit
    ) =>
    property('a);

  let assertProperty1: (arbitrary('a), 'a => unit) => unit;
  let assertProperty2:
    (arbitrary('a), arbitrary('b), ('a, 'b) => unit) => unit;
  let assertProperty3:
    (arbitrary('a), arbitrary('b), arbitrary('c), ('a, 'b, 'c) => unit) =>
    unit;
  let assertProperty4:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      ('a, 'b, 'c, 'd) => unit
    ) =>
    unit;
  let assertProperty5:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      arbitrary('e),
      ('a, 'b, 'c, 'd, 'e) => unit
    ) =>
    unit;
};

module AsyncUnit: {
  let assert_: asyncProperty('a) => Js.Promise.t(unit);
  let assertParams:
    (asyncProperty('a), Parameters.t('a)) => Js.Promise.t(unit);
  let check: asyncProperty('a) => Js.Promise.t(fcRunDetails('a));
  let checkParams:
    (asyncProperty('a), Parameters.t('a)) => Js.Promise.t(fcRunDetails('a));
  let property1:
    (arbitrary('a), 'a => Js.Promise.t(unit)) => asyncProperty('a);
  let property2:
    (arbitrary('a), arbitrary('b), ('a, 'b) => Js.Promise.t(unit)) =>
    asyncProperty('a);
  let property3:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      ('a, 'b, 'c) => Js.Promise.t(unit)
    ) =>
    asyncProperty('a);
  let property4:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      ('a, 'b, 'c, 'd) => Js.Promise.t(unit)
    ) =>
    asyncProperty('a);
  let property5:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      arbitrary('e),
      ('a, 'b, 'c, 'd, 'e) => Js.Promise.t(unit)
    ) =>
    asyncProperty('a);

  let assertProperty1:
    (arbitrary('a), 'a => Js.Promise.t(unit)) => Js.Promise.t(unit);
  let assertProperty2:
    (arbitrary('a), arbitrary('b), ('a, 'b) => Js.Promise.t(unit)) =>
    Js.Promise.t(unit);
  let assertProperty3:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      ('a, 'b, 'c) => Js.Promise.t(unit)
    ) =>
    Js.Promise.t(unit);
  let assertProperty4:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      ('a, 'b, 'c, 'd) => Js.Promise.t(unit)
    ) =>
    Js.Promise.t(unit);
  let assertProperty5:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      arbitrary('e),
      ('a, 'b, 'c, 'd, 'e) => Js.Promise.t(unit)
    ) =>
    Js.Promise.t(unit);
};