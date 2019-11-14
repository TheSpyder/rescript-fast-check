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
        "failures": array('a),
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
module FcAssert = {
  [@bs.module "fast-check"] external sync: property('a) => unit = "assert";

  [@bs.module "fast-check"]
  external async: asyncProperty('a) => unit = "assert";
};

module MakeSync = (M: {type r;}) => {
  type r = M.r;

  [@bs.module "fast-check"] external assert_: property('a) => unit = "assert";
  [@bs.module "fast-check"]
  external assertParams: (property('a), Parameters.t('a)) => unit = "assert";
  [@bs.module "fast-check"]
  external check: property('a) => fcResult('a) = "check";
  [@bs.module "fast-check"]
  external checkParams: (property('a), Parameters.t('a)) => fcResult('a) =
    "check";

  [@bs.module "fast-check"]
  external property1: (arbitrary('a), 'a => r) => property('a) = "property";

  [@bs.module "fast-check"]
  external property2:
    (arbitrary('a), arbitrary('b), ('a, 'b) => r) => property('a) =
    "property";
  [@bs.module "fast-check"]
  external property3:
    (arbitrary('a), arbitrary('b), arbitrary('c), ('a, 'b, 'c) => r) =>
    property('a) =
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
      ('a, 'b, 'c, 'd, 'e) => r
    ) =>
    property('a) =
    "property";

  let assertProperty1 = (arb, f) => assert_(property1(arb, f));
  let assertProperty2 = (arb1, arb2, f) =>
    assert_(property2(arb1, arb2, f));
  let assertProperty3 = (arb1, arb2, arb3, f) =>
    assert_(property3(arb1, arb2, arb3, f));
  let assertProperty4 = (arb1, arb2, arb3, arb4, f) =>
    assert_(property4(arb1, arb2, arb3, arb4, f));
  let assertProperty5 = (arb1, arb2, arb3, arb4, arb5, f) =>
    assert_(property5(arb1, arb2, arb3, arb4, arb5, f));
};

module MakeAsync = (M: {type r;}) => {
  type r = M.r;

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
    (arbitrary('a), 'a => Js.Promise.t(r)) => asyncProperty('a) =
    "asyncProperty";

  [@bs.module "fast-check"]
  external property2:
    (arbitrary('a), arbitrary('b), ('a, 'b) => Js.Promise.t(r)) =>
    asyncProperty('a) =
    "asyncProperty";
  [@bs.module "fast-check"]
  external property3:
    (
      arbitrary('a),
      arbitrary('b),
      arbitrary('c),
      ('a, 'b, 'c) => Js.Promise.t(r)
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
      ('a, 'b, 'c, 'd) => Js.Promise.t(r)
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
      ('a, 'b, 'c, 'd, 'e) => Js.Promise.t(r)
    ) =>
    asyncProperty('a) =
    "asyncProperty";

  let assertProperty1 = (arb, f) => assert_(property1(arb, f));
  let assertProperty2 = (arb1, arb2, f) =>
    assert_(property2(arb1, arb2, f));
  let assertProperty3 = (arb1, arb2, arb3, f) =>
    assert_(property3(arb1, arb2, arb3, f));
  let assertProperty4 = (arb1, arb2, arb3, arb4, f) =>
    assert_(property4(arb1, arb2, arb3, arb4, f));
  let assertProperty5 = (arb1, arb2, arb3, arb4, arb5, f) =>
    assert_(property5(arb1, arb2, arb3, arb4, arb5, f));
};

module Sync =
  MakeSync({
    type r = bool;
  });

module Async =
  MakeAsync({
    type r = bool;
  });

[@bs.module "fast-check"] external pre: bool => unit = "pre";

/**
 * The void/unit modules have to be manual, because BuckleScript compiles unit as `0`, not `undefined`
 *
 * The property functions only check for falsy values to trigger the boolean path, so we need to do
 * a bunch of extra work to return undefined
 *
 * NOTE: the void modules are not exposed. Only the unit wrappers.
 */
type void = Js.undefined(unit);
module SyncVoid =
  MakeSync({
    type r = void;
  });

module SyncUnit = {
  let assert_ = SyncVoid.assert_;
  let assertParams = SyncVoid.assertParams;
  let check = SyncVoid.check;
  let checkParams = SyncVoid.checkParams;

  let property1 = (arb, uf) => {
    SyncVoid.property1(
      arb,
      a => {
        uf(a);
        Js.Undefined.empty;
      },
    );
  };

  let property2 = (arb1, arb2, uf) => {
    SyncVoid.property2(
      arb1,
      arb2,
      (a, b) => {
        uf(a, b);
        Js.Undefined.empty;
      },
    );
  };

  let property3 = (arb1, arb2, arb3, uf) => {
    SyncVoid.property3(
      arb1,
      arb2,
      arb3,
      (a, b, c) => {
        uf(a, b, c);
        Js.Undefined.empty;
      },
    );
  };

  let property4 = (arb1, arb2, arb3, arb4, uf) => {
    SyncVoid.property4(
      arb1,
      arb2,
      arb3,
      arb4,
      (a, b, c, d) => {
        uf(a, b, c, d);
        Js.Undefined.empty;
      },
    );
  };

  let property5 = (arb1, arb2, arb3, arb4, arb5, uf) => {
    SyncVoid.property5(
      arb1,
      arb2,
      arb3,
      arb4,
      arb5,
      (a, b, c, d, e) => {
        uf(a, b, c, d, e);
        Js.Undefined.empty;
      },
    );
  };

  let assertProperty1 = (arb, f) => SyncVoid.assert_(property1(arb, f));
  let assertProperty2 = (arb1, arb2, f) =>
    SyncVoid.assert_(property2(arb1, arb2, f));
  let assertProperty3 = (arb1, arb2, arb3, f) =>
    SyncVoid.assert_(property3(arb1, arb2, arb3, f));
  let assertProperty4 = (arb1, arb2, arb3, arb4, f) =>
    SyncVoid.assert_(property4(arb1, arb2, arb3, arb4, f));
  let assertProperty5 = (arb1, arb2, arb3, arb4, arb5, f) =>
    SyncVoid.assert_(property5(arb1, arb2, arb3, arb4, arb5, f));
};

type asyncVoidResult = Js.undefined(unit);

module AsyncVoid =
  MakeAsync({
    type r = asyncVoidResult;
  });

module AsyncUnit = {
  let assert_ = AsyncVoid.assert_;
  let assertParams = AsyncVoid.assertParams;
  let check = AsyncVoid.check;
  let checkParams = AsyncVoid.checkParams;

  // BuckleScript promise bindings require wrapping
  let resolveUndefined = _ => Js.Promise.resolve(Js.Undefined.empty);

  let property1 = (arb, uf) => {
    AsyncVoid.property1(arb, a =>
      uf(a)->Js.Promise.then_(resolveUndefined, _)
    );
  };

  let property2 = (arb1, arb2, uf) => {
    AsyncVoid.property2(arb1, arb2, (a, b) =>
      uf(a, b)->Js.Promise.then_(resolveUndefined, _)
    );
  };

  let property3 = (arb1, arb2, arb3, uf) => {
    AsyncVoid.property3(arb1, arb2, arb3, (a, b, c) =>
      uf(a, b, c)->Js.Promise.then_(resolveUndefined, _)
    );
  };

  let property4 = (arb1, arb2, arb3, arb4, uf) => {
    AsyncVoid.property4(arb1, arb2, arb3, arb4, (a, b, c, d) =>
      uf(a, b, c, d)->Js.Promise.then_(resolveUndefined, _)
    );
  };

  let property5 = (arb1, arb2, arb3, arb4, arb5, uf) => {
    AsyncVoid.property5(arb1, arb2, arb3, arb4, arb5, (a, b, c, d, e) =>
      uf(a, b, c, d, e)->Js.Promise.then_(resolveUndefined, _)
    );
  };

  let assertProperty1 = (arb, f) => AsyncVoid.assert_(property1(arb, f));
  let assertProperty2 = (arb1, arb2, f) =>
    AsyncVoid.assert_(property2(arb1, arb2, f));
  let assertProperty3 = (arb1, arb2, arb3, f) =>
    AsyncVoid.assert_(property3(arb1, arb2, arb3, f));
  let assertProperty4 = (arb1, arb2, arb3, arb4, f) =>
    AsyncVoid.assert_(property4(arb1, arb2, arb3, arb4, f));
  let assertProperty5 = (arb1, arb2, arb3, arb4, arb5, f) =>
    AsyncVoid.assert_(property5(arb1, arb2, arb3, arb4, arb5, f));
};

// unused, probably should move to an Interfaces file
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

type asyncUnitResult = Js.Promise.t(unit);