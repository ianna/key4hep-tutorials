For RNTuple, ROOT already has native support for `std::array<T, N>`, C-style arrays, `std::vector`, `std::tuple`, `std::optional`, etc.—no need for a custom `StdArray` wrapper.

### 1. Flatten each “column” into a separate RNTuple field

RNTuple is fundamentally columnar (SoA) already, so each data column in your SoA layout should be declared like this:

```cpp
auto model = ROOT::RNTupleModel::Create();
auto colX = model->MakeField<T>("colX");
...
auto writer = ROOT::RNTupleWriter::Recreate(std::move(model), "ntpl", "out.root");
```

At each event, fill your SoA container (which may hold pointers or stride logic), then assign:

```cpp
*colX = mySoA.columnX[i];
```

If you have many columns, you can generate them via BOOST_PP loops, matching your SoA schema.

### 2. Handling multi-element columns (arrays within rows)

If a column itself holds fixed-size spans or arrays (e.g. Eigen vectors or `std::array<float, 4>`), RNTuple supports that:

```cpp
auto arrF = model->MakeField<std::array<float, 4>>("arrF");
```

You’d then do:

```cpp
std::array<float,4> local = {...};
*arrF = local;
```
So your SoA stride/pointer wrappers remain in memory, but RNTuple only sees flattened fields. Fixed-size multidimensional arrays are fully supported.

### 3. Reflection and dictionaries for custom types

Only user-defined types need ROOT dictionaries (via rootcling/LinkDef). Since you're only persisting standard types and fixed-size arrays, you don’t need custom dictionaries—just use `std::array`, `T`, `std::span` stays local.

### 4. What remains of your SoA library?

You can keep all your CUDA/SOA pointer & iterator logic, just treat it as an in-memory computation helper. Before filling each RNTuple entry:

  * Prepare your data in SoA layout.

  * Copy each column into the corresponding RNTuple field pointer.

  * Call `writer->Fill()`.

So your SoA code isn't serialized, only its final data is.
