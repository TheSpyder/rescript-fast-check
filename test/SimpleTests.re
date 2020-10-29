/*
  This replicates some tests from bs-jsverify:
  https://github.com/Risto-Stevcev/bs-jsverify/blob/master/test/Test_Verify.re

  (at least as many as we're able to given it hasn't all been replicated in these bindings)
 */

open BsMocha.Mocha;
open Arbitrary;
open Property.Sync;
open Combinators;

describe("Various simple tests", () => {
  // boolean test case is optimised by the compiler and pointless

  it("nat `+` (associative)", () =>
    assertProperty3(nat(), nat(), nat(), (n1, n2, n3) => n1 + n2 + n3 == n1 + (n2 + n3))
  );

  it("null(nat)", () =>
    assertProperty1(null(nat()), n =>
      switch (Js.Null.toOption(n)) {
      | Some(num) => num >= 0
      | None => true
      }
    )
  );

  it("option(nat)", () =>
    assertProperty1(
      Combinators.option(nat()),
      fun
      | Some(num) => num >= 0
      | None => true,
    )
  );

  it("sum of nats is >= 0", () => {
    // clearly jsverify doesn't generate natural numbers close to the maximum JS supports
    // fast-check does so we need to limit it
    assertProperty1(array(nat(~max=200, ())), a => {Array.fold_left((+), 0, a) >= 0});

    assertProperty1(list(nat(~max=200, ())), l => {List.fold_left((+), 0, l) >= 0});
  });

  it("testing tuple", () => {
    assertProperty1(tuple2(nat(~max=200, ()), nat(~max=200, ())), ((a, b)) => {
      a + b >= a && a + b >= b
    })
  });
});
