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
  [@deriving {abstract: light}]
  type t('a) = {
    [@optional]
    endOnFailure: bool,
    [@optional]
    examples: array('a),
    [@optional]
    interruptAfterTimeLimit: int,
    [@optional]
    logger: string => unit,
    [@optional]
    markInterruptAsFailure: bool,
    [@optional]
    maxSkipsPerRun: int,
    [@optional]
    numRuns: int,
    [@optional]
    path: string,
    // NOT MAPPED [@optional] randomType - this looks hard
    [@optional]
    seed: int,
    [@optional]
    skipAllAfterTimeLimit: int,
    [@optional]
    timeout: int,
    [@optional]
    unbiased: bool,
    [@optional]
    verbose: bool // todo this blocks VeryVerbose
  };
};

/**  */
module type Sync = {
  type r;

  [@module "fast-check"] external assert_: property('a) => unit = "assert";
  [@module "fast-check"]
  external assertParams: (property('a), Parameters.t('a)) => unit = "assert";
  [@module "fast-check"] external check: property('a) => fcRunDetails('a) = "check";
  [@module "fast-check"]
  external checkParams: (property('a), Parameters.t('a)) => fcRunDetails('a) = "check";

  /**
   * Property types are always arrays, but you can't have a tuple of one.
   *
   * So property1 just hard codes array('a).
   */
  [@module "fast-check"]
  external property1: (arbitrary('a), 'a => r) => property(array('a)) = "property";

  [@module "fast-check"]
  external property2: (arbitrary('a), arbitrary('b), ('a, 'b) => r) => property(('a, 'b)) =
    "property";
  [@module "fast-check"]
  external property3:
    (arbitrary('a), arbitrary('b), arbitrary('c), ('a, 'b, 'c) => r) => property(('a, 'b, 'c)) =
    "property";

  [@module "fast-check"]
  external property4:
    (arbitrary('a), arbitrary('b), arbitrary('c), arbitrary('d), ('a, 'b, 'c, 'd) => r) =>
    property(('a, 'b, 'c, 'd)) =
    "property";

  [@module "fast-check"]
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

  [@send] external beforeEach: (property('a), unit => unit) => property('a) = "beforeEach";
  [@send] external afterEach: (property('a), unit => unit) => property('a) = "afterEach";

  // Convenience combining `assert` and `property` into a single function call
  let assertProperty1: (arbitrary('a), 'a => r) => unit;
  let assertProperty2: (arbitrary('a), arbitrary('b), ('a, 'b) => r) => unit;
  let assertProperty3: (arbitrary('a), arbitrary('b), arbitrary('c), ('a, 'b, 'c) => r) => unit;
  let assertProperty4:
    (arbitrary('a), arbitrary('b), arbitrary('c), arbitrary('d), ('a, 'b, 'c, 'd) => r) => unit;
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

  [@module "fast-check"] external assert_: asyncProperty('a) => Js.Promise.t(unit) = "assert";
  [@module "fast-check"]
  external assertParams: (asyncProperty('a), Parameters.t('a)) => Js.Promise.t(unit) = "assert";
  [@module "fast-check"]
  external check: asyncProperty('a) => Js.Promise.t(fcRunDetails('a)) = "check";
  [@module "fast-check"]
  external checkParams: (asyncProperty('a), Parameters.t('a)) => Js.Promise.t(fcRunDetails('a)) =
    "check";

  /**
   * Property types are always arrays, but you can't do a tuple of one.
   *
   * So property1 just hard codes array('a).
   */
  [@module "fast-check"]
  external property1: (arbitrary('a), 'a => Js.Promise.t(r)) => asyncProperty(array('a)) =
    "asyncProperty";

  [@module "fast-check"]
  external property2:
    (arbitrary('a), arbitrary('b), ('a, 'b) => Js.Promise.t(r)) => asyncProperty(('a, 'b)) =
    "asyncProperty";
  [@module "fast-check"]
  external property3:
    (arbitrary('a), arbitrary('b), arbitrary('c), ('a, 'b, 'c) => Js.Promise.t(r)) =>
    asyncProperty(('a, 'b, 'c)) =
    "asyncProperty";

  [@module "fast-check"]
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

  [@module "fast-check"]
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

  [@send]
  external beforeEach: (asyncProperty('a), unit => unit) => asyncProperty('a) = "beforeEach";
  [@send]
  external afterEach: (asyncProperty('a), unit => unit) => asyncProperty('a) = "afterEach";

  // Convenience combining `assert` and `property` into a single function call
  let assertProperty1: (arbitrary('a), 'a => Js.Promise.t(r)) => Js.Promise.t(unit);
  let assertProperty2:
    (arbitrary('a), arbitrary('b), ('a, 'b) => Js.Promise.t(r)) => Js.Promise.t(unit);
  let assertProperty3:
    (arbitrary('a), arbitrary('b), arbitrary('c), ('a, 'b, 'c) => Js.Promise.t(r)) =>
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
