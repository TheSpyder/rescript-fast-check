// ported from the fast-check getting started guide
// https://github.com/dubzzz/fast-check#getting-started
open BsMocha.Mocha;

// open FastCheck;
open Arbitrary;
open Property.Sync;

// Code under test
let contains = (text, pattern) => text->Js.String2.indexOf(pattern) >= 0;

// Properties
describe("properties", () => {
  // string text always contains itself
  it("should always contain itself", () => {
    assertProperty1(string(), text => contains(text, text))
  });
  // string a + b + c always contains b, whatever the values of a, b and c
  it("should always contain its substrings", () => {
    assertProperty3(string(), string(), string(), (a, b, c) => {contains(a ++ b ++ c, b)})
  });
});
