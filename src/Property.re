include Intf_Property;

/* TODO not well documented so unsure how to bind:
   TIPS 1:

   The output of property and asyncProperty (respectively Property and AsyncProperty) accepts optional beforeEach and afterEach hooks that would be invoked before and after the execution of the predicate.

   property(arb1, predicate)
       .beforeEach(() => { /* code executed before each call to predicate */ })
       .afterEach(() => { /* code executed after each call to predicate */ });

   asyncProperty(arb1, predicate)
       .beforeEach(async () => { /* code executed before each call to predicate */ })
       .afterEach(async () => { /* code executed after each call to predicate */ });

   */

[@bs.get] external hasFailed: fcRunDetails('a) => bool = "failed";

let toResult = r => hasFailed(r) ? Failed(Obj.magic(r)) : Passed;


module FcAssert = {
  [@bs.module "fast-check"] external sync: property('a) => unit = "assert";

  [@bs.module "fast-check"]
  external async: asyncProperty('a) => Js.Promise.t(unit) = "assert";
};

[@bs.module "fast-check"] external pre: bool => unit = "pre";

module MakeSync = (M: {type r;}) => {
  type r = M.r;

  [@bs.module "fast-check"] external assert_: property('a) => unit = "assert";
  [@bs.module "fast-check"]
  external assertParams: (property('a), Parameters.t('a)) => unit = "assert";
  [@bs.module "fast-check"]
  external check: property('a) => fcRunDetails('a) = "check";
  [@bs.module "fast-check"]
  external checkParams: (property('a), Parameters.t('a)) => fcRunDetails('a) =
    "check";

  /**
   * Property types are always arrays, but you can't do a tuple of one.
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

  // more convenience methods to avoid dancing around the "assert is a keyword" issue
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

  // more convenience methods to avoid dancing around the "assert is a keyword" issue
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
module SyncUnit =
  MakeSync({
    type r = unit;
  });

module Async =
  MakeAsync({
    type r = bool;
  });
module AsyncUnit =
  MakeAsync({
    type r = unit;
  });
