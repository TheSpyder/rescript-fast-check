open BsMocha.Mocha;
open Arbitrary;
open Property;

let testParams =
  Parameters.t(
    ~endOnFailure=true,
    ~examples=[|true|],
    ~interruptAfterTimeLimit=5,
    ~markInterruptAsFailure=true,
    ~maxSkipsPerRun=5,
    ~numRuns=3,
    ~path="",
    ~seed=3,
    ~skipAllAfterTimeLimit=5,
    ~timeout=999,
    ~unbiased=false,
    ~verbose=false,
    (),
  );

// TODO: make each test do a simple verification that the arbitrary API calls have mapped correctly
describe("sync property checks", () => {
  let eq = i => i === i;
  let eq2 = (i, j) => i === i && j === j;
  let eq3 = (i, j, k) => i === i && j === j && k === k;
  let eq4 = (i, j, k, l) => i === i && j === j && k === k && l === l;
  let eq5 = (i, j, k, l, m) =>
    i === i && j === j && k === k && l === l && m === m;
  open Property.Sync;
  it("sync assert_", () => {
    assert_(property1(boolean(), eq));
    assert_(property2(boolean(), boolean(), eq2));
    assert_(property3(boolean(), boolean(), boolean(), eq3));
    assert_(property4(boolean(), boolean(), boolean(), boolean(), eq4));
    assert_(
      property5(boolean(), boolean(), boolean(), boolean(), boolean(), eq5),
    );
  });
  it("sync assert / check params", () => {
    assertParams(property1(boolean(), eq), testParams);

    let checkResult = check(property1(boolean(), eq));
    Js.log(checkResult);
    switch (toResult(checkResult)) {
    | Passed => ()
    | Failed(r) =>
      let _ = r##counterexample;
      let _ = r##counterexamplePath;
      let _ = r##error;
      let _ = r##failures;
      let _ = r##numRuns;
      let _ = r##numShrinks;
      let _ = r##numSkips;
      let _ = r##seed;
      ();
    };

    let _ = checkParams(property1(boolean(), eq), testParams);

    pre(true);
  });
  it("sync FcAssert", () => {
    FcAssert.sync(property1(boolean(), eq));
    FcAssert.sync(property2(boolean(), boolean(), eq2));
    FcAssert.sync(property3(boolean(), boolean(), boolean(), eq3));
    FcAssert.sync(
      property4(boolean(), boolean(), boolean(), boolean(), eq4),
    );
    FcAssert.sync(
      property5(boolean(), boolean(), boolean(), boolean(), boolean(), eq5),
    );
  });
  it("sync assertProperty", () => {
    assertProperty1(boolean(), eq);
    assertProperty2(boolean(), boolean(), eq2);
    assertProperty3(boolean(), boolean(), boolean(), eq3);
    assertProperty4(boolean(), boolean(), boolean(), boolean(), eq4);
    assertProperty5(
      boolean(),
      boolean(),
      boolean(),
      boolean(),
      boolean(),
      eq5,
    );
  });
});

describe("syncUnit property checks", () => {
  let eq = _ => ();
  let eq2 = (_, _) => ();
  let eq3 = (_, _, _) => ();
  let eq4 = (_, _, _, _) => ();
  let eq5 = (_, _, _, _, _) => ();
  open Property.SyncUnit;
  it("syncUnit assert_", () => {
    assert_(property1(boolean(), eq));
    assert_(property2(boolean(), boolean(), eq2));
    assert_(property3(boolean(), boolean(), boolean(), eq3));
    assert_(property4(boolean(), boolean(), boolean(), boolean(), eq4));
    assert_(
      property5(boolean(), boolean(), boolean(), boolean(), boolean(), eq5),
    );
  });
  it("syncUnit assert / check params", () => {
    assertParams(property1(boolean(), eq), testParams);

    let checkResult = check(property1(boolean(), eq));
    Js.log(checkResult);
    switch (toResult(checkResult)) {
    | Passed => ()
    | Failed(r) =>
      let _ = r##counterexample;
      let _ = r##counterexamplePath;
      let _ = r##error;
      let _ = r##failures;
      let _ = r##numRuns;
      let _ = r##numShrinks;
      let _ = r##numSkips;
      let _ = r##seed;
      ();
    };

    let _ = checkParams(property1(boolean(), eq), testParams);

    pre(true);
  });
  it("syncUnit FcAssert", () => {
    FcAssert.sync(property1(boolean(), eq));
    FcAssert.sync(property2(boolean(), boolean(), eq2));
    FcAssert.sync(property3(boolean(), boolean(), boolean(), eq3));
    FcAssert.sync(
      property4(boolean(), boolean(), boolean(), boolean(), eq4),
    );
    FcAssert.sync(
      property5(boolean(), boolean(), boolean(), boolean(), boolean(), eq5),
    );
  });
  it("syncUnit assertProperty", () => {
    assertProperty1(boolean(), eq);
    assertProperty2(boolean(), boolean(), eq2);
    assertProperty3(boolean(), boolean(), boolean(), eq3);
    assertProperty4(boolean(), boolean(), boolean(), boolean(), eq4);
    assertProperty5(
      boolean(),
      boolean(),
      boolean(),
      boolean(),
      boolean(),
      eq5,
    );
  });
});