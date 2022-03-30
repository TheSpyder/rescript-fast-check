// ported from the fast-check getting started guide
// https://github.com/dubzzz/fast-check#getting-started
open RescriptMocha.Mocha

// open FastCheck;
open Arbitrary
open Property.Sync

// Code under test
let contains = (text, pattern) => text->Js.String2.indexOf(pattern) >= 0

// Properties
describe("properties", () => {
  // string text always contains itself
  it("should always contain itself", () =>
    assert_(property1(string(), text => contains(text, text)))
  )
  // string a + b + c always contains b, whatever the values of a, b and c
  it("should always contain its substrings", () =>
    assert_(property3(string(), string(), string(), (a, b, c) => contains(a ++ (b ++ c), b)))
  )
})

// Recommended alternative: use a convenience method to avoid the weird `assert_` name
describe("properties using rescript-fast-check shorthand", () => {
  it("should always contain itself", () => assertProperty1(string(), text => contains(text, text)))
  it("should always contain its substrings", () =>
    assertProperty3(string(), string(), string(), (a, b, c) => contains(a ++ (b ++ c), b))
  )
})
