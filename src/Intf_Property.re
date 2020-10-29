type arbitrary('a) = Arbitrary.arbitrary('a);

type property('a);
type asyncProperty('a);

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

// using types to hide the fact that the same object is returned in all cases
type fcRunDetails('a);
// this can't be an inline record because we use Obj.magic to convert to it
type fcFailureDetails('a) = {
  counterexample: 'a,
  counterexamplePath: string,
  error: string,
  failures: array('a),
  // NOT MAPPED executionSummary tbd this looks hard
  numRuns: int,
  numShrinks: int,
  numSkips: int,
  seed: int,
  runConfiguration: Parameters.t('a),
};
type runDetails('a) =
  | Passed
  | Failed(fcFailureDetails('a));

module type CommonMethods = {
  type pReturn;
  type propertyType('a);
  type assertReturn;
  type checkReturn('a);

  [@module "fast-check"] external assert_: propertyType('a) => assertReturn = "assert";
  [@module "fast-check"]
  external assertParams: (propertyType('a), Parameters.t('a)) => assertReturn = "assert";
  [@module "fast-check"] external check: propertyType('a) => checkReturn('a) = "check";
  [@module "fast-check"]
  external checkParams: (propertyType('a), Parameters.t('a)) => checkReturn('a) = "check";

  // Convenience combining `assert` and `property` into a single function call
  let assertProperty1: (arbitrary('a), 'a => pReturn) => assertReturn;
  let assertProperty2: (arbitrary('a), arbitrary('b), ('a, 'b) => pReturn) => assertReturn;
  let assertProperty3:
    (arbitrary('a), arbitrary('b), arbitrary('c), ('a, 'b, 'c) => pReturn) => assertReturn;
  let assertProperty4:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      ('a, 'b, 'c, 'd) => pReturn
    ) =>
    assertReturn;
  let assertProperty5:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      arbitrary('e),
      ('a, 'b, 'c, 'd, 'e) => pReturn
    ) =>
    assertReturn;
};

/**  */
module type Sync = {
  type predicateReturn;

  include
    CommonMethods with
      type pReturn := predicateReturn and
      type propertyType('a) := property('a) and
      type assertReturn := unit and
      type checkReturn('a) := fcRunDetails('a);

  /**
   * Property types are always arrays, but you can't have a tuple of one.
   *
   * So property1 just hard codes array('a).
   */
  [@module "fast-check"]
  external property1: (arbitrary('a), 'a => predicateReturn) => property(array('a)) =
    "property";

  [@module "fast-check"]
  external property2:
    (arbitrary('a), arbitrary('b), ('a, 'b) => predicateReturn) => property(('a, 'b)) =
    "property";
  [@module "fast-check"]
  external property3:
    (arbitrary('a), arbitrary('b), arbitrary('c), ('a, 'b, 'c) => predicateReturn) =>
    property(('a, 'b, 'c)) =
    "property";

  [@module "fast-check"]
  external property4:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      ('a, 'b, 'c, 'd) => predicateReturn
    ) =>
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
      ('a, 'b, 'c, 'd, 'e) => predicateReturn
    ) =>
    property(('a, 'b, 'c, 'd, 'e)) =
    "property";

  [@send] external beforeEach: (property('a), unit => unit) => property('a) = "beforeEach";
  [@send] external afterEach: (property('a), unit => unit) => property('a) = "afterEach";
};

module type Async = {
  type predicateReturn;

  include
    CommonMethods with
      type pReturn := Js.Promise.t(predicateReturn) and
      type propertyType('a) := asyncProperty('a) and
      type assertReturn := Js.Promise.t(unit) and
      type checkReturn('a) := Js.Promise.t(fcRunDetails('a));

  /**
   * Property types are always arrays, but you can't do a tuple of one.
   *
   * So property1 just hard codes array('a).
   */
  [@module "fast-check"]
  external property1:
    (arbitrary('a), 'a => Js.Promise.t(predicateReturn)) => asyncProperty(array('a)) =
    "asyncProperty";

  [@module "fast-check"]
  external property2:
    (arbitrary('a), arbitrary('b), ('a, 'b) => Js.Promise.t(predicateReturn)) =>
    asyncProperty(('a, 'b)) =
    "asyncProperty";
  [@module "fast-check"]
  external property3:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      ('a, 'b, 'c) => Js.Promise.t(predicateReturn)
    ) =>
    asyncProperty(('a, 'b, 'c)) =
    "asyncProperty";

  [@module "fast-check"]
  external property4:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      arbitrary('d),
      ('a, 'b, 'c, 'd) => Js.Promise.t(predicateReturn)
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
      ('a, 'b, 'c, 'd, 'e) => Js.Promise.t(predicateReturn)
    ) =>
    asyncProperty(('a, 'b, 'c, 'd, 'e)) =
    "asyncProperty";

  [@send]
  external beforeEach: (asyncProperty('a), unit => unit) => asyncProperty('a) = "beforeEach";
  [@send]
  external afterEach: (asyncProperty('a), unit => unit) => asyncProperty('a) = "afterEach";
};
