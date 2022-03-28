include Intf_Property

@get external hasFailed: fcRunDetails<'a> => bool = "failed"

let toResult = r => hasFailed(r) ? Failed(Obj.magic(r)) : Passed

module FcAssert = {
  @module("fast-check") external sync: property<'a> => unit = "assert"

  @module("fast-check") external async: asyncProperty<'a> => Js.Promise.t<unit> = "assert"
}

@module("fast-check") external pre: bool => unit = "pre"

module MakeSync = (
  M: {
    type predicateReturn
  },
) => {
  type predicateReturn = M.predicateReturn
  // The OCaml type system requires this to be a new type. I'm not sure why.
  type assertReturn = unit

  @module("fast-check") external assert_: property<'a> => assertReturn = "assert"
  @module("fast-check")
  external assertParams: (property<'a>, Parameters.t<'a>) => assertReturn = "assert"
  @module("fast-check") external check: property<'a> => fcRunDetails<'a> = "check"
  @module("fast-check")
  external checkParams: (property<'a>, Parameters.t<'a>) => fcRunDetails<'a> = "check"

  @ocaml.doc("
   * Property types are always arrays, but you can't have a tuple of one.
   *
   * So property1 just hard codes array('a).
   ")
  @module("fast-check")
  external property1: (arbitrary<'a>, 'a => predicateReturn) => property<array<'a>> = "property"

  @module("fast-check")
  external property2: (
    arbitrary<'a>,
    arbitrary<'b>,
    ('a, 'b) => predicateReturn,
  ) => property<('a, 'b)> = "property"
  @module("fast-check")
  external property3: (
    arbitrary<'a>,
    arbitrary<'b>,
    arbitrary<'c>,
    ('a, 'b, 'c) => predicateReturn,
  ) => property<('a, 'b, 'c)> = "property"

  @module("fast-check")
  external property4: (
    arbitrary<'a>,
    arbitrary<'b>,
    arbitrary<'c>,
    arbitrary<'d>,
    ('a, 'b, 'c, 'd) => predicateReturn,
  ) => property<('a, 'b, 'c, 'd)> = "property"

  @module("fast-check")
  external property5: (
    arbitrary<'a>,
    arbitrary<'b>,
    arbitrary<'c>,
    arbitrary<'d>,
    arbitrary<'e>,
    ('a, 'b, 'c, 'd, 'e) => predicateReturn,
  ) => property<('a, 'b, 'c, 'd, 'e)> = "property"

  @send external beforeEach: (property<'a>, unit => unit) => property<'a> = "beforeEach"
  @send external afterEach: (property<'a>, unit => unit) => property<'a> = "afterEach"

  // convenience methods to avoid dancing around the "assert is a keyword" issue
  let assertProperty1 = (arb, f) => assert_(property1(arb, f))
  let assertProperty2 = (arb1, arb2, f) => assert_(property2(arb1, arb2, f))
  let assertProperty3 = (arb1, arb2, arb3, f) => assert_(property3(arb1, arb2, arb3, f))
  let assertProperty4 = (arb1, arb2, arb3, arb4, f) => assert_(property4(arb1, arb2, arb3, arb4, f))
  let assertProperty5 = (arb1, arb2, arb3, arb4, arb5, f) =>
    assert_(property5(arb1, arb2, arb3, arb4, arb5, f))
}

module MakeAsync = (
  M: {
    type predicateReturn
  },
) => {
  type predicateReturn = M.predicateReturn

  @module("fast-check") external assert_: asyncProperty<'a> => Js.Promise.t<unit> = "assert"
  @module("fast-check")
  external assertParams: (asyncProperty<'a>, Parameters.t<'a>) => Js.Promise.t<unit> = "assert"
  @module("fast-check")
  external check: asyncProperty<'a> => Js.Promise.t<fcRunDetails<'a>> = "check"
  @module("fast-check")
  external checkParams: (asyncProperty<'a>, Parameters.t<'a>) => Js.Promise.t<fcRunDetails<'a>> =
    "check"

  @ocaml.doc("
   * Property types are always arrays, but you can't do a tuple of one.
   *
   * So property1 just hard codes array('a).
   ")
  @module("fast-check")
  external property1: (
    arbitrary<'a>,
    'a => Js.Promise.t<predicateReturn>,
  ) => asyncProperty<array<'a>> = "asyncProperty"

  @module("fast-check")
  external property2: (
    arbitrary<'a>,
    arbitrary<'b>,
    ('a, 'b) => Js.Promise.t<predicateReturn>,
  ) => asyncProperty<('a, 'b)> = "asyncProperty"
  @module("fast-check")
  external property3: (
    arbitrary<'a>,
    arbitrary<'b>,
    arbitrary<'c>,
    ('a, 'b, 'c) => Js.Promise.t<predicateReturn>,
  ) => asyncProperty<('a, 'b, 'c)> = "asyncProperty"

  @module("fast-check")
  external property4: (
    arbitrary<'a>,
    arbitrary<'b>,
    arbitrary<'c>,
    arbitrary<'d>,
    ('a, 'b, 'c, 'd) => Js.Promise.t<predicateReturn>,
  ) => asyncProperty<('a, 'b, 'c, 'd)> = "asyncProperty"

  @module("fast-check")
  external property5: (
    arbitrary<'a>,
    arbitrary<'b>,
    arbitrary<'c>,
    arbitrary<'d>,
    arbitrary<'e>,
    ('a, 'b, 'c, 'd, 'e) => Js.Promise.t<predicateReturn>,
  ) => asyncProperty<('a, 'b, 'c, 'd, 'e)> = "asyncProperty"

  @send
  external beforeEach: (asyncProperty<'a>, unit => unit) => asyncProperty<'a> = "beforeEach"
  @send
  external afterEach: (asyncProperty<'a>, unit => unit) => asyncProperty<'a> = "afterEach"

  // convenience methods to avoid dancing around the "assert is a keyword" issue
  let assertProperty1 = (arb, f) => assert_(property1(arb, f))
  let assertProperty2 = (arb1, arb2, f) => assert_(property2(arb1, arb2, f))
  let assertProperty3 = (arb1, arb2, arb3, f) => assert_(property3(arb1, arb2, arb3, f))
  let assertProperty4 = (arb1, arb2, arb3, arb4, f) => assert_(property4(arb1, arb2, arb3, arb4, f))
  let assertProperty5 = (arb1, arb2, arb3, arb4, arb5, f) =>
    assert_(property5(arb1, arb2, arb3, arb4, arb5, f))
}

module Sync = MakeSync({
  type predicateReturn = bool
})
module SyncUnit = MakeSync({
  type predicateReturn = unit
})

module Async = MakeAsync({
  type predicateReturn = bool
})
module AsyncUnit = MakeAsync({
  type predicateReturn = unit
})
