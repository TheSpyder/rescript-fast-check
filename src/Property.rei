include (module type of Intf_Property);

[@bs.get] external hasFailed: fcRunDetails('a) => bool = "failed";

let toResult: fcRunDetails('a) => runDetails('b);

/**
 * Convenience module for anyone who doesn't like the function name `assert_` (assert is a keyword in ReasonML)
 */
module FcAssert: {
  [@bs.module "fast-check"] external sync: property('a) => unit = "assert";

  [@bs.module "fast-check"]
  external async: asyncProperty('a) => Js.Promise.t(unit) = "assert";
};

[@bs.module "fast-check"] external pre: bool => unit = "pre";

// Do these first to avoid weird results from shadowing
module SyncUnit: Sync with type r = unit;
module AsyncUnit: Async with type r = unit;

module Sync: Sync with type r = bool;
module Async: Async with type r = bool;