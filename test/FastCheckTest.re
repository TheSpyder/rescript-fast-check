open BsMocha.Mocha;
open Arbitrary;
open Property;
open Property.Sync;

let id = i => i == i;

// TODO: make each test do a simple verification that the arbitrary API calls have mapped correctly
describe("arbitraries", () => {
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
  it("float", () =>
    // FcAssert.sync(property1(float(), id));
    FcAssert.sync(
      property1(floatRange(0., 5.), id),
    )
  );
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
    FcAssert.sync(property1(hexaStringRange(0, 5), id));
    FcAssert.sync(property1(base64String(), id));
    FcAssert.sync(property1(base64StringRange(0, 5), id));
    FcAssert.sync(property1(string(), id));
    FcAssert.sync(property1(stringRange(0, 5), id));
    FcAssert.sync(property1(asciiString(), id));
    FcAssert.sync(property1(asciiStringRange(0, 5), id));
    FcAssert.sync(property1(string16Bits(), id));
    FcAssert.sync(property1(string16BitsRange(0, 5), id));
    FcAssert.sync(property1(fullUnicodeString(), id));
    FcAssert.sync(property1(fullUnicodeStringRange(0, 5), id));
    FcAssert.sync(property1(stringOf(ascii()), id));
    FcAssert.sync(property1(stringOfRange(ascii(), 0, 5), id));
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
  it("combinators of arbitraries", () => {
    FcAssert.sync(property1(array(hexa()), id));
    FcAssert.sync(property1(constant(hexa()), id));
    FcAssert.sync(property1(constantFrom([|1, 2, 3, 4|]), id));
    FcAssert.sync(property1(oneOf([|integer(), nat(), |]), id));
    FcAssert.sync(property1(oneOf([|hexaString(), asciiString(), base64String()|]), id));
  });
  it("methods on arbitraries", () => {
    FcAssert.sync(property1(map(hexa(), (s) => s++s), id));
    FcAssert.sync(property1(chain(hexa(), constant), id));
  });
})