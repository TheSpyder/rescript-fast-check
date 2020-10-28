# rescript-fast-check

[fast-check](https://github.com/dubzzz/fast-check) bindings for ReScript

## Installation

```
npm i --save-dev res-fast-check
```
or
```
yarn add -D res-fast-check
```

Then add rescript-fast-check to `bsconfig.json`:
```
  "bs-dev-dependencies": [
    "rescript-fast-check"
  ],
```

## Usage

Everything is under the `FastCheck` namespace. The API is roughly mapped to the fast-check documentation, although not everything lines up as well as it used to after fast-check had a big documentation restructure in September 2020.

### `FastCheck.Arbitrary`
This module covers [built-in arbitraries](https://github.com/dubzzz/fast-check/blob/master/documentation/Arbitraries.md) and [custom arbitraries](https://github.com/dubzzz/fast-check/blob/master/documentation/AdvancedArbitraries.md). Top level functions are simple arbitraries that generate values with no (or very little) input, sub-modules are used to group more complicated arbitraries together.

* `Combinators` contains most of the APIs grouped under [combinators](https://github.com/dubzzz/fast-check/blob/master/documentation/Arbitraries.md#combinators) in the fast-check documentation.
* `Objects` groups the [objects](https://github.com/dubzzz/fast-check/blob/master/documentation/Arbitraries.md#object) and letrec APIs, which are technically `Combinators` but a bit more advanced.
* `Derive` includes most of the [custom arbitrary APIs](https://github.com/dubzzz/fast-check/blob/master/documentation/AdvancedArbitraries.md#derive-existing-arbitraries), the ones that derive arbitraries from arbitraries.


## Contributing

There are a few places that could use assistance.

* Better tests to validate that the values produced by the API calls are correct. The existing tests only validate that bindings are able to call APIs without a runtime error.
* More complete fast-check API bindings. Major things are all covered, nost of the missing parts are due to difficulty in figuring out a type-safe binding. Brand new APIs may also lack bindings if I haven't noticed they were added.
* Add doc comments to every binding, to aid autocomplete results. Do this by copying descriptions from the fast-check [documentation](https://github.com/dubzzz/fast-check/tree/master/documentation) or [API reference](https://dubzzz.github.io/fast-check/), whichever provides the better description.
* Consider converting from ReasonML syntax to ReScript. However until the [rescript-vscode](https://github.com/rescript-lang/rescript-vscode) extension is complete I'm likely to prefer ReasonML due to the better development experience. It makes no difference which syntax this repository uses when it's used as a library.
* If there is enough feedback that the binding module structure differences to fast-check documentation are confusing, a big refactor to fix that would be welcome.

## License

All code is licensed as BSD. See [LICENSE](LICENSE).

Some development for this repository was done on work time by employees at [Tiny](https://github.com/tinymce/) but all of the code is released under the BSD license.