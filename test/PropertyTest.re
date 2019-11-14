open BsMocha.Mocha;
open Arbitrary;
open Property;

let testParams =
  Parameters.t(
    ~endOnFailure=true,
    ~examples=[|true|],
    ~interruptAfterTimeLimit=999,
    ~logger=s => Js.log(s),
    ~markInterruptAsFailure=true,
    ~maxSkipsPerRun=999,
    ~numRuns=1,
    ~path="",
    ~seed=3,
    ~skipAllAfterTimeLimit=999,
    ~timeout=999,
    ~unbiased=false,
    ~verbose=true,
    (),
  );

let validateRunDetails = cr =>
  switch (toResult(cr)) {
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

    check(property1(boolean(), eq))->validateRunDetails;

    checkParams(property1(boolean(), eq), testParams)->validateRunDetails;

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

    check(property1(boolean(), eq))->validateRunDetails;

    checkParams(property1(boolean(), eq), testParams)->validateRunDetails;

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

open! BsMocha.Promise;
describe("async property checks", () => {
  let eq = i => Js.Promise.resolve(i === i);
  let eq2 = (i, j) => Js.Promise.resolve(i === i && j === j);
  let eq3 = (i, j, k) => Js.Promise.resolve(i === i && j === j && k === k);
  let eq4 = (i, j, k, l) =>
    Js.Promise.resolve(i === i && j === j && k === k && l === l);
  let eq5 = (i, j, k, l, m) =>
    Js.Promise.resolve(i === i && j === j && k === k && l === l && m === m);
  open Property.Async;
  it("async assert_ 1", () =>
    assert_(property1(boolean(), eq))
  );
  it("async assert_ 2", () =>
    assert_(property2(boolean(), boolean(), eq2))
  );
  it("async assert_ 3", () =>
    assert_(property3(boolean(), boolean(), boolean(), eq3))
  );
  it("async assert_ 4", () =>
    assert_(property4(boolean(), boolean(), boolean(), boolean(), eq4))
  );
  it("async assert_ 5", () =>
    assert_(
      property5(boolean(), boolean(), boolean(), boolean(), boolean(), eq5),
    )
  );
  it("async assert / check params 1", () =>
    assertParams(property1(boolean(), eq), testParams)
  );
  it("async assert / check params 2", () => {
    pre(true);
    check(property1(boolean(), eq))
    |> Js.Promise.then_(checkResult => {
         validateRunDetails(checkResult);
         Js.Promise.resolve();
       });
  });
  it("async assert / check params 3", () => {
    pre(true);
    checkParams(property1(boolean(), eq), testParams)
    |> Js.Promise.then_(checkResult => {
         validateRunDetails(checkResult);
         Js.Promise.resolve();
       });
  });
  it("async FcAssert 1", () =>
    FcAssert.async(property1(boolean(), eq))
  );
  it("async FcAssert 2", () =>
    FcAssert.async(property2(boolean(), boolean(), eq2))
  );
  it("async FcAssert 3", () =>
    FcAssert.async(property3(boolean(), boolean(), boolean(), eq3))
  );
  it("async FcAssert 4", () =>
    FcAssert.async(
      property4(boolean(), boolean(), boolean(), boolean(), eq4),
    )
  );
  it("async FcAssert 5", () =>
    FcAssert.async(
      property5(boolean(), boolean(), boolean(), boolean(), boolean(), eq5),
    )
  );
  it("async assertProperty 1", () =>
    assertProperty1(boolean(), eq)
  );
  it("async assertProperty 1", () =>
    assertProperty2(boolean(), boolean(), eq2)
  );
  it("async assertProperty 1", () =>
    assertProperty3(boolean(), boolean(), boolean(), eq3)
  );
  it("async assertProperty 1", () =>
    assertProperty4(boolean(), boolean(), boolean(), boolean(), eq4)
  );
  it("async assertProperty 1", () =>
    assertProperty5(
      boolean(),
      boolean(),
      boolean(),
      boolean(),
      boolean(),
      eq5,
    )
  );
});

describe("asyncUnit property checks", () => {
  let eq = _ => Js.Promise.resolve();
  let eq2 = (_, _) => Js.Promise.resolve();
  let eq3 = (_, _, _) => Js.Promise.resolve();
  let eq4 = (_, _, _, _) => Js.Promise.resolve();
  let eq5 = (_, _, _, _, _) => Js.Promise.resolve();
  open Property.AsyncUnit;
  it("async assert_ 1", () =>
    assert_(property1(boolean(), eq))
  );
  it("async assert_ 2", () =>
    assert_(property2(boolean(), boolean(), eq2))
  );
  it("async assert_ 3", () =>
    assert_(property3(boolean(), boolean(), boolean(), eq3))
  );
  it("async assert_ 4", () =>
    assert_(property4(boolean(), boolean(), boolean(), boolean(), eq4))
  );
  it("async assert_ 5", () =>
    assert_(
      property5(boolean(), boolean(), boolean(), boolean(), boolean(), eq5),
    )
  );
  it("async assert / check params 1", () =>
    assertParams(property1(boolean(), eq), testParams)
  );
  it("async assert / check params 2", () => {
    pre(true);
    check(property1(boolean(), eq))
    |> Js.Promise.then_(checkResult => {
         validateRunDetails(checkResult);
         Js.Promise.resolve();
       });
  });
  it("async assert / check params 3", () => {
    pre(true);
    checkParams(property1(boolean(), eq), testParams)
    |> Js.Promise.then_(checkResult => {
         validateRunDetails(checkResult);
         Js.Promise.resolve();
       });
  });
  it("async FcAssert 1", () =>
    FcAssert.async(property1(boolean(), eq))
  );
  it("async FcAssert 2", () =>
    FcAssert.async(property2(boolean(), boolean(), eq2))
  );
  it("async FcAssert 3", () =>
    FcAssert.async(property3(boolean(), boolean(), boolean(), eq3))
  );
  it("async FcAssert 4", () =>
    FcAssert.async(
      property4(boolean(), boolean(), boolean(), boolean(), eq4),
    )
  );
  it("async FcAssert 5", () =>
    FcAssert.async(
      property5(boolean(), boolean(), boolean(), boolean(), boolean(), eq5),
    )
  );
  it("async assertProperty 1", () =>
    assertProperty1(boolean(), eq)
  );
  it("async assertProperty 1", () =>
    assertProperty2(boolean(), boolean(), eq2)
  );
  it("async assertProperty 1", () =>
    assertProperty3(boolean(), boolean(), boolean(), eq3)
  );
  it("async assertProperty 1", () =>
    assertProperty4(boolean(), boolean(), boolean(), boolean(), eq4)
  );
  it("async assertProperty 1", () =>
    assertProperty5(
      boolean(),
      boolean(),
      boolean(),
      boolean(),
      boolean(),
      eq5,
    )
  );
});