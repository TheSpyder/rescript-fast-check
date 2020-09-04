/*
 * Don't use this module, BuckleScript just doesn't let me hide it when using namespaces
 */
type arbitrary('a) = Arbitrary.arbitrary('a);

type property('a);
type asyncProperty('a);

// using types to hide the fact that the same object is returned in all cases
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
        // "executionSummary" tbd this looks hard
        "numRuns": int,
        "numShrinks": int,
        "numSkips": int,
        "seed": int,
        // "verbose" doesn't have a mapping yet
      },
    );

module Parameters = {
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
    // NOT MAPPED [@bs.optional] randomType - this looks hard
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

/**  */
module type Sync = {
  type r;

  [@bs.module "fast-check"] external assert_: property('a) => unit = "assert";
  [@bs.module "fast-check"]
  external assertParams: (property('a), Parameters.t('a)) => unit = "assert";
  [@bs.module "fast-check"]
  external check: property('a) => fcRunDetails('a) = "check";
  [@bs.module "fast-check"]
  external checkParams: (property('a), Parameters.t('a)) => fcRunDetails('a) =
    "check";

  /**
   * Property types are always arrays, but you can't have a tuple of one.
   *
   * So property1 just hard codes array('a).
   */
  [@bs.module "fast-check"]
  external property1: (arbitrary('a), 'a => r) => property(array('a)) =
    "property";

  [@bs.module "fast-check"]
  external property2:
    (arbitrary('a), arbitrary('b), ('a, 'b) => r) => property(('a, 'b)) =
    "property";
  [@bs.module "fast-check"]
  external property3:
    (arbitrary('a), arbitrary('b), arbitrary('c), ('a, 'b, 'c) => r) =>
    property(('a, 'b, 'c)) =
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
    property(('a, 'b, 'c, 'd)) =
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
    property(('a, 'b, 'c, 'd, 'e)) =
    "property";

  // Convenience combining `assert` and `property` into a single function call
  let assertProperty1: (arbitrary('a), 'a => r) => unit;
  let assertProperty2:
    (arbitrary('a), arbitrary('b), ('a, 'b) => r) => unit;
  let assertProperty3:
    (arbitrary('a), arbitrary('b), arbitrary('c), ('a, 'b, 'c) => r) =>
    unit;
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


module type Async = {
  type r;

  [@bs.module "fast-check"]
  external assert_: asyncProperty('a) => Js.Promise.t(unit) = "assert";
  [@bs.module "fast-check"]
  external assertParams:
    (asyncProperty('a), Parameters.t('a)) => Js.Promise.t(unit) =
    "assert";
  [@bs.module "fast-check"]
  external check: asyncProperty('a) => Js.Promise.t(fcRunDetails('a)) =
    "check";
  [@bs.module "fast-check"]
  external checkParams:
    (asyncProperty('a), Parameters.t('a)) => Js.Promise.t(fcRunDetails('a)) =
    "check";

  /**
   * Property types are always arrays, but you can't do a tuple of one.
   *
   * So property1 just hard codes array('a).
   */
  [@bs.module "fast-check"]
  external property1:
    (arbitrary('a), 'a => Js.Promise.t(r)) => asyncProperty(array('a)) =
    "asyncProperty";

  [@bs.module "fast-check"]
  external property2:
    (arbitrary('a), arbitrary('b), ('a, 'b) => Js.Promise.t(r)) =>
    asyncProperty(('a, 'b)) =
    "asyncProperty";
  [@bs.module "fast-check"]
  external property3:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      ('a, 'b, 'c) => Js.Promise.t(r)
    ) =>
    asyncProperty(('a, 'b, 'c)) =
    "asyncProperty";

  [@bs.module "fast-check"]
  external property4:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      ('a, 'b, 'c, 'd) => Js.Promise.t(r)
    ) =>
    asyncProperty(('a, 'b, 'c, 'd)) =
    "asyncProperty";

  [@bs.module "fast-check"]
  external property5:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      arbitrary('e),
      ('a, 'b, 'c, 'd, 'e) => Js.Promise.t(r)
    ) =>
    asyncProperty(('a, 'b, 'c, 'd, 'e)) =
    "asyncProperty";

  // Convenience combining `assert` and `property` into a single function call
  let assertProperty1:
    (arbitrary('a), 'a => Js.Promise.t(r)) => Js.Promise.t(unit);
  let assertProperty2:
    (arbitrary('a), arbitrary('b), ('a, 'b) => Js.Promise.t(r)) =>
    Js.Promise.t(unit);
  let assertProperty3:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      ('a, 'b, 'c) => Js.Promise.t(r)
    ) =>
    Js.Promise.t(unit);
  let assertProperty4:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      ('a, 'b, 'c, 'd) => Js.Promise.t(r)
    ) =>
    Js.Promise.t(unit);
  let assertProperty5:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      arbitrary('e),
      ('a, 'b, 'c, 'd, 'e) => Js.Promise.t(r)
    ) =>
    Js.Promise.t(unit);
};