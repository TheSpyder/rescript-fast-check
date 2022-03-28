/*
 TODO
 In order to define the default parameters that will be used by runners you can use one of the following helpers:

 fc.configureGlobal(parameters: GlobalParameters): define the default parameters to be used by runners
 fc.resetConfigureGlobal(): reset the default parameters to be used by runners
 fc.readConfigureGlobal(): output the default parameters to be used by runners

 */
/*


 [@module "fast-check"]
 external sampleArb:
   (~arb: Arbitrary.arbitrary('a), ~genSize: int=?, unit) => array('a) =
   "sample";
 [@module "fast-check"]
 external sampleProperty:
   (~arb: Property.property('a), ~genSize: int=?, unit) => array('a) =
   "sample";

 fc.sample: sample generated values of an Arbitrary<T> or Property<T>
 It builds an array containing all the values that would have been generated for the equivalent test.

 It also accept Parameters as configuration in order to help you diagnose the shape of the inputs that will be received by your property.

 type Generator<Ts> = Arbitrary<Ts> | IProperty<Ts>;

 function sample<Ts>(generator: Generator<Ts>): Ts[];
 function sample<Ts>(generator: Generator<Ts>, params: Parameters): Ts[];
 function sample<Ts>(generator: Generator<Ts>, numGenerated: number): Ts[];

 */
/*

 fc.statistics: classify the values produced by an Arbitrary<T> or Property<T>
 It provides useful statistics concerning generated values. In order to be able to gather those statistics it has to be provided with a classifier function that can classify the generated value in zero, one or more categories (free labels).

 It also accept Parameters as configuration in order to help you diagnose the shape of the inputs that will be received by your property.

 Statistics are dumped into console.log but can be redirected to another source by modifying the logger key in Parameters.

 type Generator<Ts> = Arbitrary<Ts> | IProperty<Ts>;
 type Classifier<Ts> = ((v: Ts) => string) | ((v: Ts) => string[]);

 function statistics<Ts>(generator: Generator<Ts>, classify: Classifier<Ts>): void;
 function statistics<Ts>(generator: Generator<Ts>, classify: Classifier<Ts>, params: Parameters): void;
 function statistics<Ts>(generator: Generator<Ts>, classify: Classifier<Ts>, numGenerated: number): void;

 */
