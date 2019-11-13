open BsMocha.Mocha;
open Arbitrary;
open Property;
open Property.Sync;

let id = i => i === i;

// TODO: make each test do a simple verification that the arbitrary API calls have mapped correctly
describe("primitive built-in arbitraries", () => {
  it("bool", () =>
    FcAssert.sync(property1(boolean(), id))
  );
  it("int", () => {
    FcAssert.sync(property1(integer(), id));
    FcAssert.sync(property1(integerRange(0, 5), id));
    FcAssert.sync(property1(maxSafeInteger(), id));
  });
  it("nat", () => {
    FcAssert.sync(property1(nat(), id));
    FcAssert.sync(property1(nat(~max=5, ()), id));
    FcAssert.sync(property1(maxSafeNat(), id));
  });
  it("float", () => {
    // float needs a name that doesn't conflict with Pervasives
    // FcAssert.sync(property1(float(), id));
    FcAssert.sync(property1(floatRange(0., 5.), id));
    FcAssert.sync(property1(floatRange(0., 10.), id));
  });
  it("double", () => {
    FcAssert.sync(property1(double(), id));
    FcAssert.sync(property1(doubleRange(0., 5.), id));
  });
  it("characters", () => {
    FcAssert.sync(property1(char(), id));
    FcAssert.sync(property1(char16bits(), id));
    FcAssert.sync(property1(hexa(), id));
    FcAssert.sync(property1(base64(), id));
    FcAssert.sync(property1(ascii(), id));
    FcAssert.sync(property1(fullUnicode(), id));
  });
  it("string", () => {
    FcAssert.sync(property1(hexaString(), id));
    FcAssert.sync(property1(hexaStringWithLength(0, 5), id));
    FcAssert.sync(property1(base64String(), id));
    FcAssert.sync(property1(base64StringWithLength(0, 5), id));
    FcAssert.sync(property1(string(), id));
    FcAssert.sync(property1(stringWithLength(0, 5), id));
    FcAssert.sync(property1(asciiString(), id));
    FcAssert.sync(property1(asciiStringWithLength(0, 5), id));
    FcAssert.sync(property1(string16Bits(), id));
    FcAssert.sync(property1(string16BitsWithLength(0, 5), id));
    FcAssert.sync(property1(fullUnicodeString(), id));
    FcAssert.sync(property1(fullUnicodeStringWithLength(0, 5), id));
    FcAssert.sync(property1(stringOf(ascii()), id));
    FcAssert.sync(property1(stringOfWithLength(ascii(), 0, 5), id));
  });
  it("date", () => {
    FcAssert.sync(property1(date(), id));
    FcAssert.sync(
      property1(
        dateRange({
          "min": Js.Date.fromFloat(0.),
          "max": Js.Date.fromFloat(354894654.),
        }),
        id,
      ),
    );
  });
});

describe("combinators", () => {
  open Combinators;
  it("methods on arbitraries", () => {
    FcAssert.sync(property1(map(hexa(), s => s ++ s), id));
    FcAssert.sync(property1(filter(hexa(), _ => true), id));
    FcAssert.sync(property1(chain(hexa(), constant), id));
    FcAssert.sync(property1(noShrink(hexa()), id));
    FcAssert.sync(property1(noBias(hexa()), id));
  });

  it("combinators of arbitraries", () => {
    FcAssert.sync(property1(constant(hexa()), id));
    FcAssert.sync(property1(constantFrom([|1, 2, 3, 4|]), id));
    FcAssert.sync(
      property1(
        // example from the docs
        mapToConstant([|
          {"num": 26, "build": v => Js.String.fromCharCode(v + 0x61)},
          {"num": 10, "build": v => Js.String.fromCharCode(v + 0x30)},
        |]),
        id,
      ),
    );
    FcAssert.sync(property1(oneOf([|integer(), nat()|]), id));
    FcAssert.sync(
      property1(oneOf([|hexaString(), asciiString(), base64String()|]), id),
    );
    FcAssert.sync(property1(null(hexa()), id));
    FcAssert.sync(
      property1(subArray([|1, 2, 3, 4, 5, 6, 7, 8, 9, 10|]), id),
    );
    FcAssert.sync(
      property1(
        subArrayWithLength([|1, 2, 3, 4, 5, 6, 7, 8, 9, 10|], 5, 10),
        id,
      ),
    );
    FcAssert.sync(
      property1(shuffledSubArray([|1, 2, 3, 4, 5, 6, 7, 8, 9, 10|]), id),
    );
    FcAssert.sync(
      property1(
        shuffledSubArrayWithLength([|1, 2, 3, 4, 5, 6, 7, 8, 9, 10|], 5, 10),
        id,
      ),
    );
    FcAssert.sync(property1(array(hexa()), id));
    FcAssert.sync(property1(arrayWithLength(hexa(), 5, 10), id));
    FcAssert.sync(property1(set(hexa()), id));
    FcAssert.sync(
      property1(setWithLength(hexa(), 5, 10, ~comparator=(===)), id),
    );
    FcAssert.sync(property1(tuple2(hexa(), hexa()), id));
    FcAssert.sync(property1(tuple3(hexa(), hexa(), hexa()), id));
    FcAssert.sync(property1(tuple4(hexa(), hexa(), hexa(), hexa()), id));
    FcAssert.sync(
      property1(tuple5(hexa(), hexa(), hexa(), hexa(), hexa()), id),
    );
    FcAssert.sync(property1(dictionary(hexa(), fullUnicodeString()), id));

    let recordGenerator = Js.Dict.empty();
    Js.Dict.set(recordGenerator, "a", hexaString());
    Js.Dict.set(recordGenerator, "b", hexaString());
    Js.Dict.set(recordGenerator, "c", hexaString());
    FcAssert.sync(property1(record(recordGenerator, ~withDeletedKeys=true), id));
    FcAssert.sync(property1(dedup(hexa(), 5), id));
  });
});

describe("ReasonML specific combinators", () => {
  open Combinators;
  it("cons list", () => {
    FcAssert.sync(property1(list(hexa()), id));
    FcAssert.sync(property1(list(integer()), id));
  });
  it("option", () => {
    FcAssert.sync(property1(option(hexa()), id));
    FcAssert.sync(property1(option(integer()), id));
  });
});