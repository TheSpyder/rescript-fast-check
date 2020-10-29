# rescript-fast-check

[fast-check](https://github.com/dubzzz/fast-check) bindings for ReScript

## Installation

```
npm i --save-dev rescript-fast-check
```
or
```
yarn add -D rescript-fast-check
```

Then add rescript-fast-check to `bsconfig.json`:
```
  "bs-dev-dependencies": [
    "rescript-fast-check"
  ],
```

## Example

Ported from the [fast-check example](https://github.com/dubzzz/fast-check#getting-started), using bs-mocha

```re
open FastCheck
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
    assert_(
      property3(string(), string(), string(), (a, b, c) =>
        contains(a ++ b ++ c, b)
      ),
    )
  )
})
```

For further examples please browse the tests. The tests are included in the NPM package for this purpose.

## Goals

These bindings aim to be zero cost as much as possible. The library is almost exclusively `external` bindings, with a handful of methods for convenience and helping to map edge cases to the ReScript type system. The above example compiles to JavaScript that is semantically identical to the fast-check example it was ported from; this can be seen by compiling the [example test file](test/ExampleTest.re).

## Usage

Everything is under the `FastCheck` namespace. Rather than use a flat namespace, as the fast-check API does, ReScript affords us the ability to use some logical module groupings while still compiling to the flat namespace.

The `rescript-fast-check` API groups are roughly mapped to the fast-check documentation, although not everything lines up as well as it used to after some of the fast-check documentation was restructured.

### `FastCheck.Arbitrary`
This module covers [built-in arbitraries](https://github.com/dubzzz/fast-check/blob/master/documentation/Arbitraries.md) and [custom arbitraries](https://github.com/dubzzz/fast-check/blob/master/documentation/AdvancedArbitraries.md). Top level functions are simple arbitraries that generate values with no (or very little) input, sub-modules are used to group more complicated arbitraries together.

* `Combinators` contains most of the APIs grouped under [combinators](https://github.com/dubzzz/fast-check/blob/master/documentation/Arbitraries.md#combinators) in the fast-check documentation.
* `Objects` groups the [objects](https://github.com/dubzzz/fast-check/blob/master/documentation/Arbitraries.md#object) and [letrec](https://github.com/dubzzz/fast-check/blob/master/documentation/Arbitraries.md#recursive-structures) APIs, which are technically also combinators but a bit more advanced to work within the ReScript type system.
* `Derive` includes most of the [custom arbitrary APIs](https://github.com/dubzzz/fast-check/blob/master/documentation/AdvancedArbitraries.md#derive-existing-arbitraries), the ones that derive arbitraries from arbitraries.
* `Scheduler` is a dedicated module for `fc.scheduler()`, for information on this see the [detect race conditions](https://github.com/dubzzz/fast-check/blob/master/documentation/RaceConditions.md) documentation.
* `Context` groups methods for using `fc.context()`, for more information on this see the [log within a predicate](https://github.com/dubzzz/fast-check/blob/master/documentation/Tips.md#log-within-a-predicate) documentation.

### `FastCheck.Property`

To maintain type safety, this library does not bind with varadic arguments. Property test bindings are available up to 5 arguments; if you need more a PR would be welcome.

Fast-check is very strict about the return types of property testing methods. If a predicate does not return `undefined`, a truthy check is used to validate the response. ReScript does not have a convenient mapping for returning `bool | unit` without jumping through hoops so two modules are used for each of the sync / async styles.

Property testing is therefore split into 4 sub-modules with an identical list of methods covering the [property based runners](https://github.com/dubzzz/fast-check/blob/master/documentation/Runners.md):
* `Sync` for synchronous boolean checks
* `SyncUnit` for synchronous exception-throwing checks
* `Async` for promise-based checks that resolve to a boolean
* `AsyncUnit` for promise-based checks that reject in case of failure

Because I was a JSVerify fan I have added a helper method to combine `assert` and `property1` into a single function call, `assertProperty1`, a feature JSVerify offers but fast-check does not. This is implemented in each of the 4 property testing sub-modules, also extending to 5 property arguments. The [example test file](test/ExampleTest.re) shows the difference.

As a further inconvenience, `assert` is a keyword in ReScript so the binding was renamed to `assert_`. This means a zero cost binding to a single property assertion is `assert_(property1(arb, f))`. To improve the developer experience, two options are provided:
1. Te recommended approach is the helper method `assertProperty1`, but it is not zero cost.
2. The `Property.FcAssert` sub-module contains `sync` and `async`, which are zero-cost bindings. The example then becomes `FcAssert.sync(property1(arb, f))`.

## Contributing

There are a lot of places that could use assistance! Contributions are very welcome. Here are some suggestions:

* Move the usage section of this file to a proper documentation area and include more examples.
* Add doc comments to every binding to aid autocomplete results. Do this by copying descriptions from the fast-check [documentation](https://github.com/dubzzz/fast-check/tree/master/documentation) or [API reference](https://dubzzz.github.io/fast-check/), whichever provides the better description.
* Doc comments would also mean an automated documentation tool can be added.
* Better tests to validate that the values produced by the API calls match the binding types. The existing tests only validate that bindings are able to call APIs without a runtime error.
* More complete fast-check API bindings. Most of the missing parts are due to difficulty in figuring out a type-safe binding. Brand new APIs may also lack bindings if I haven't noticed they were added.
* As an example, `FastCheck.Global` is intended to map to fast-check's [global configuration options](https://github.com/dubzzz/fast-check/blob/master/documentation/Runners.md#global-configuration) but it is unimplemented.
* Consider converting from ReasonML syntax to ReScript. However until the [rescript-vscode](https://github.com/rescript-lang/rescript-vscode) extension is complete I'm likely to prefer ReasonML due to the better development experience. It makes no difference which syntax this repository uses when installed as a dependency.
* If there is enough feedback that the binding module structure differences to fast-check documentation are confusing, a big refactor to line them all up again would be necessary.

## Implementation notes

The insistence on zero-cost bindings results in a lot of duplication in the `Property` module. Functors are used to avoid duplication between `bool` and `unit` property checks, but the design of `external` types requires duplication between `fc.property` definitions and `fc.assertProperty`. The only methods where duplication can be avoided are `fc.assert` and `fc.check`, which doesn't seem worth the extra hassle (but they are de-duplicated in the `Intf_Property` module types).

## License and Credits

All code is licensed as BSD. See [LICENSE](LICENSE).

The original inspiration for this project came from [bs-jsverify](https://github.com/Risto-Stevcev/bs-jsverify) after jsverify was abandoned.

Some development has been done during work hours by employees at [Tiny](https://github.com/tinymce/).