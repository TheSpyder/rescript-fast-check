include module type of Intf_Property

@get external hasFailed: fcRunDetails<'a> => bool = "failed"

let toResult: fcRunDetails<'a> => runDetails<'b>

@ocaml.doc("
 * Convenience module for anyone who doesn't like the function name `assert_` (assert is a keyword in ReasonML)
 ")
module FcAssert: {
  @module("fast-check") external sync: property<'a> => unit = "assert"

  @module("fast-check") external async: asyncProperty<'a> => Js.Promise.t<unit> = "assert"
}

@module("fast-check") external pre: bool => unit = "pre"

module SyncUnit: Sync with type predicateReturn := unit
module AsyncUnit: Async with type predicateReturn := unit

module Sync: Sync with type predicateReturn := bool
module Async: Async with type predicateReturn := bool
