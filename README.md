## Fast Vector Fields 

FastVectorFields is a compact library for storing, operating and modifying 2 and 3-dimensional vector fields.
Such fields can be used to store velocities, trajectories, densities and other vector data. Can be used for simulation in physics, flow fields
in games or visualization.

If you, like me, were looking for such a library but did not find anything quite matching the use-case, then you have found the right library.

### What are vector fields?

A vector field is a common concept in physical/mathematical modeling
that describes an object that assigns a point in space to a vector value.

#### For 2D:
$` \vec F(x, y) = P(x,y) \vec i + Q(x,y) \vec j`$

#### For 3D: 
$` \vec F(x, y, z) = P(x,y,z) \vec i + Q(x,y,z) \vec j + R(x,y,z) \vec k`$


### What data structures do you have to deal with? 

The library📚 contains its own mathematics structs and functions to avoid collision with your maths libraries.

`namespace vfMath` contains the vectors you are going to need for the library
`vfMath:Vector2D<T>` and `vfMath:Vector3D<T>`, where T is bound to number types.

`namespace vfFields` contains all the fields that you are going to need. 

- `AbstractField2D` and `AbstractField3D` are used to give an abstraction and separate common properties from their implementation.

- `ScalarField2D` and `ScalarField3D` represent fields with scalar values. They can be transformed to a corresponding VectorField using gradient.

- `VectorField2D` and `VectorField3D` are exactly the fields you came here for. 

Thanks to the implemented cereal library, you can serialize and deserialize 
all the mentioned above classes.

### How to use the library?

```cpp
#include <iostream>
#include <fstream>
#include <cereal/types/memory.hpp>

#include "../VectorFields/VectorField2D.h"
#include "../ScalarFields/ScalarField2D.h"
#include "../FieldBase/AbstractField2D.h"
#include "../Vectors/Vector2D.h"
#include  "../Vectors/Vector3D.h"

using vfMath::Vector2D;

using vfFields::VectorField2D;
using vfFields::ScalarField2D;

int main()
{
    ScalarField2D<double> scalar_field(128, 128);

    scalar_field.fill([](const double x, const double y)
    {
        const double r = std::sqrt(x * x + y * y);
        const double theta = std::atan2(y, x);
        return std::sin(8 * M_PI * r + 4 * theta);
    }, -1.0, 1.0, -1.0, 1.0);

    VectorField2D vector_field(scalar_field);
    vector_field.normalize();

    std::ofstream os1("scalar_field.json", std::ios::binary);
    cereal::JSONOutputArchive archive1(os1);

    archive1(cereal::make_nvp<>("scalar_field", scalar_field));

    std::ofstream os("vector_field.json", std::ios::binary);
    cereal::JSONOutputArchive archive(os);
    archive(cereal::make_nvp<>("vector_field", vector_field));

    return 0;
}
```

This code produces two `.json` files that contain two serialized (row-major)
fields. They can be visualized using matplotlib or plotly charts.

#### For 2D:


| Field type       | High resolution                                                                               | Low resolution                                                                          |
|------------------|-----------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------|
| **Scalar field** | <img src="https://i.imgur.com/IgstqDF.png" alt="https://imgur.com/a/szF2nss" width="768">     | <img src="https://i.imgur.com/qMfzTqw.png" alt="https://imgur.com/qMfzTqw" width="768"> |
| **Vector field** | <img src="https://i.imgur.com/FUdcHgT.png" alt="https://i.imgur.com/FUdcHgT.png" width="768"> | <img src="https://i.imgur.com/EmnDeYz.png" alt="https://imgur.com/EmnDeYz" width="768"> |

#### For 3D:

| Field type       | High resolution                                                                           | Low resolution                                                                            |
|------------------|-------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------|
| **Scalar field** | <img src="https://i.imgur.com/3fz93rL.png" alt="https://imgur.com/3fz93rL" width="768"> | <img src="https://i.imgur.com/4e8NJWP.png" alt="https://imgur.com/4e8NJWP" width="768">   |
| **Vector field** | <img src="https://i.imgur.com/mz7nOdS.png" alt="https://imgur.com/mz7nOdS" width="768">   | <img src="https://i.imgur.com/7D3kfRG.png" alt="https://imgur.com/7D3kfRG" width="768"> |


### Interpolation feature

The FastVectorFields library supports interpolation in two different contexts:

- Interpolation sampling

Uses bilinear or trilinear interpolation to sample between the tiles.

```cpp
VectorField2D<double> a(3, 3);
a.setValue(0, 0, {1.0, 1.0});
a.setValue(1, 1, {-1.0, -1.0});
auto result = a(0.5, 0.5);
 ```

Use case: getting additional samples between the tiles.

- Interpolation restoration ?

Uses RBF-interpolation to fill the data structure with approximated vectors.
```cpp
VectorField2D<double> a(16, 16);
a.setValue(0, 0, {1.0, 0.0});
a.setValue(3, 3, {-1.0, 0});
a.setValue(1, 1, {0.5, -0.5});
a.fillWithInterpolation();
```
Use case: filling in the gaps between the vectors(at least 3).

The purpose of this method is to produce vector fields similar to this one based on a limited number of entries.


| Field type                   | Input (Initial Vectors)                                                                   | Output (Complete Vector field)                                                            |
|------------------------------|-------------------------------------------------------------------------------------------|-------------------------------------------------------------------------------------------|
| **Graphical representation** | <img src="https://i.imgur.com/UDPPSFy.png" alt="https://imgur.com/a/szF2nss" width="768"> | <img src="https://i.imgur.com/WzAtzrL.png" alt="https://imgur.com/a/szF2nss" width="768"> |

