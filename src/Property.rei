type arbitrary('a) = BsFastCheck.Arbitrary.arbitrary('a);
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
module FcAssert: {
  [@bs.module "fast-check"] external sync: property('a) => unit = "assert";

  [@bs.module "fast-check"]
  external async: asyncProperty('a) => unit = "assert";
};

module type FcProperty = {
  type t('a);
  type r;

  [@bs.module "fast-check"] external assert_: t('a) => unit = "assert";
  [@bs.module "fast-check"]
  external assertParams: (property('a), Parameters.t('a)) => unit = "assert";
  [@bs.module "fast-check"] external check: t('a) => result('a) = "check";
  [@bs.module "fast-check"]
  external checkParams: (t('a), Parameters.t('a)) => result('a) = "check";

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

module Sync: FcProperty with type t('a) := property('a) and type r := bool;

module SyncUnit:
  FcProperty with type t('a) := property('a) and type r := unit;

type asyncResult = Js.Promise.t(bool);
module ASync:
  FcProperty with type t('a) := asyncProperty('a) and type r := asyncResult;

type asyncUnitResult = Js.Promise.t(unit);
module ASyncUnit:
  FcProperty with type t('a) := asyncProperty('a) and type r := asyncUnitResult;