open BsMocha.Mocha;
open Arbitrary;
open Property;
open Property.Sync;

let eq = i => i === i;

// TODO: make each test do a simple verification that the arbitrary API calls have mapped correctly
describe("primitive built-in arbitraries", () => {
  it("bool", () =>
    FcAssert.sync(property1(boolean(), eq))
  );
  it("int", () => {
    FcAssert.sync(property1(integer(), eq));
    FcAssert.sync(property1(integerRange(0, 5), eq));
    FcAssert.sync(property1(maxSafeInteger(), eq));
  });
  it("nat", () => {
    FcAssert.sync(property1(nat(), eq));
    FcAssert.sync(property1(nat(~max=5, ()), eq));
    FcAssert.sync(property1(maxSafeNat(), eq));
  });
  it("float", () => {
    FcAssert.sync(property1(float_(), eq));
    FcAssert.sync(property1(floatRange(0., 5.), eq));
    FcAssert.sync(property1(floatRange(0., 10.), eq));
  });
  it("double", () => {
    FcAssert.sync(property1(double(), eq));
    FcAssert.sync(property1(doubleRange(0., 5.), eq));
  });
  it("characters", () => {
    FcAssert.sync(property1(char(), eq));
    FcAssert.sync(property1(char16bits(), eq));
    FcAssert.sync(property1(hexa(), eq));
    FcAssert.sync(property1(base64(), eq));
    FcAssert.sync(property1(ascii(), eq));
    FcAssert.sync(property1(fullUnicode(), eq));
    FcAssert.sync(property1(unicode(), eq));
  });

  it("string", () => {
    FcAssert.sync(property1(hexaString(), eq));
    FcAssert.sync(property1(hexaStringWithLength(0, 5), eq));
    FcAssert.sync(property1(base64String(), eq));
    FcAssert.sync(property1(base64StringWithLength(0, 5), eq));
    FcAssert.sync(property1(string(), eq));
    FcAssert.sync(property1(stringWithLength(0, 5), eq));
    FcAssert.sync(property1(asciiString(), eq));
    FcAssert.sync(property1(asciiStringWithLength(0, 5), eq));
    FcAssert.sync(property1(string16Bits(), eq));
    FcAssert.sync(property1(string16BitsWithLength(0, 5), eq));
    FcAssert.sync(property1(fullUnicodeString(), eq));
    FcAssert.sync(property1(fullUnicodeStringWithLength(0, 5), eq));
    FcAssert.sync(property1(unicodeString(), eq));
    FcAssert.sync(property1(unicodeStringWithLength(0, 5), eq));
    FcAssert.sync(property1(stringOf(ascii()), eq));
    FcAssert.sync(property1(stringOfWithLength(ascii(), 0, 5), eq));
  });
  it("date", () => {
    FcAssert.sync(property1(date(), eq));
    FcAssert.sync(
      property1(
        dateRange({
          dateMin: Js.Date.fromFloat(0.),
          dateMax: Js.Date.fromFloat(354894654.),
        }),
        eq,
      ),
    );
  });
});

describe("combinators", () => {
  open Combinators;
  it("methods on arbitraries", () => {
    open Derive;
    FcAssert.sync(property1(map(hexa(), s => s ++ s), eq));
    FcAssert.sync(property1(filter(hexa(), _ => true), eq));
    FcAssert.sync(property1(chain(hexa(), constant), eq));
    FcAssert.sync(property1(noShrink(hexa()), eq));
    FcAssert.sync(property1(noBias(hexa()), eq));
  });

  it("combinators of arbitraries", () => {
    FcAssert.sync(property1(constant(hexa()), eq));
    FcAssert.sync(property1(constantFrom([|1, 2, 3, 4|]), eq));
    FcAssert.sync(
      property1(
        // example from the docs
        mapToConstant([|
          {"num": 26, "build": v => Js.String.fromCharCode(v + 0x61)},
          {"num": 10, "build": v => Js.String.fromCharCode(v + 0x30)},
        |]),
        eq,
      ),
    );
    FcAssert.sync(property1(oneOf([|integer(), nat()|]), eq));
    FcAssert.sync(
      property1(oneOf([|hexaString(), asciiString(), base64String()|]), eq),
    );
    FcAssert.sync(property1(null(hexa()), eq));
    FcAssert.sync(
      property1(subArray([|1, 2, 3, 4, 5, 6, 7, 8, 9, 10|]), eq),
    );
    FcAssert.sync(
      property1(
        subArrayWithLength([|1, 2, 3, 4, 5, 6, 7, 8, 9, 10|], 5, 10),
        eq,
      ),
    );
    FcAssert.sync(
      property1(shuffledSubArray([|1, 2, 3, 4, 5, 6, 7, 8, 9, 10|]), eq),
    );
    FcAssert.sync(
      property1(
        shuffledSubArrayWithLength([|1, 2, 3, 4, 5, 6, 7, 8, 9, 10|], 5, 10),
        eq,
      ),
    );
    FcAssert.sync(property1(array(hexa()), eq));
    FcAssert.sync(property1(arrayWithLength(hexa(), 5, 10), eq));
    FcAssert.sync(property1(set(hexa()), eq));
    FcAssert.sync(
      property1(setWithLength(hexa(), 5, 10, ~comparator=(===)), eq),
    );
    FcAssert.sync(property1(tuple2(hexa(), hexa()), eq));
    FcAssert.sync(property1(tuple3(hexa(), hexa(), hexa()), eq));
    FcAssert.sync(property1(tuple4(hexa(), hexa(), hexa(), hexa()), eq));
    FcAssert.sync(
      property1(tuple5(hexa(), hexa(), hexa(), hexa(), hexa()), eq),
    );
    FcAssert.sync(
      property1(
        dictionary(hexa(), fullUnicodeString())
        ->Derive.filter(o => Js.Dict.keys(o)->Array.length > 0),
        eq,
      ),
    );

    let recordGenerator = Js.Dict.empty();
    Js.Dict.set(recordGenerator, "a", hexaString());
    Js.Dict.set(recordGenerator, "b", hexaString());
    Js.Dict.set(recordGenerator, "c", hexaString());
    FcAssert.sync(
      property1(record(recordGenerator, ~withDeletedKeys=true), eq),
    );
    FcAssert.sync(property1(dedup(hexa(), 5), eq));
  });
});

describe("ReasonML specific combinators", () => {
  open Combinators;
  it("cons list", () => {
    FcAssert.sync(property1(list(hexa()), eq));
    FcAssert.sync(property1(list(integer()), eq));
  });
  it("option", () => {
    FcAssert.sync(property1(option(hexa()), eq));
    FcAssert.sync(property1(option(integer()), eq));
  });
});

describe("complex built-in arbitraries", () => {
  let constTrue = _ => true; // because `NaN === NaN` returns false
  it("object", () => {
    FcAssert.sync(property1(Objects.anything(), constTrue));
    FcAssert.sync(
      property1(
        Objects.anything(~settings=Objects.settings(~maxDepth=5, ()), ()),
        constTrue,
      ),
    );
    FcAssert.sync(property1(Objects.object_(), constTrue));
    FcAssert.sync(
      property1(
        Objects.object_(~settings=Objects.settings(~maxDepth=5, ()), ()),
        constTrue,
      ),
    );
  });

  it("json", () => {
    // Despite the names, these are really *any* JSON value
    FcAssert.sync(property1(Objects.jsonObject(5), eq));
    FcAssert.sync(property1(Objects.unicodeJsonObject(5), eq));
    let checkProducesJsonValue = arb => {
      open Js.Json;
      let checkProduces = kind =>
        SyncUnit.property1(arb, v => Property.pre(v->test(kind)))
        ->SyncUnit.assertParams(
            Parameters.t(~maxSkipsPerRun=1000, ~numRuns=1, ()),
          );
      checkProduces(String);
      checkProduces(Number);
      checkProduces(Object);
      checkProduces(Array);
      checkProduces(Boolean);
      checkProduces(Null);
    };
    checkProducesJsonValue(Objects.jsonObject(1));
    checkProducesJsonValue(Objects.unicodeJsonObject(1));
  });

  it("letrec", () => {
    // example from the docs
    let recursive =
      Objects.letrec(tie => {
        let types = Js.Dict.empty();
        Js.Dict.set(
          types,
          "tree",
          Combinators.oneOf([|
            tie(. "node"),
            tie(. "tree"),
            tie(. "leaf"),
          |])
          ->Objects.anyArb,
        );
        Js.Dict.set(
          types,
          "node",
          Combinators.tuple2(tie(. "tree"), tie(. "tree"))->Objects.anyArb,
        );
        Js.Dict.set(types, "leaf", nat()->Objects.anyArb);
        types;
      });
    let tree = Js.Dict.get(recursive, "tree")->Belt.Option.getExn;
    FcAssert.sync(property1(tree, constTrue));
  });
});
