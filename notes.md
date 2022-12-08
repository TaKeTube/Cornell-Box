## [More accurate Fresnel approximation for a RGB-based Path Tracer](https://computergraphics.stackexchange.com/questions/4877/more-accurate-fresnel-approximation-for-a-rgb-based-path-tracer)

## [Microfacet Importance Sampling: For Dummies](https://www.gamedev.net/blogs/entry/2261786-microfacet-importance-sampling-for-dummies/)





#### 40

UNIFORM

```c++
(Vector3f(1.f) - F)*lambert + cook_torrance
```

颜色显然不正常

#### 41

UNIFORM

```C++
(1.f - max(F))*lambert + cook_torrance
```

颜色正常

#### 42

IS_BRDF

```C++
(1.f - max(F))*lambert + cook_torrance
```

全白 炸了

#### 43

IS_BRDF

pdf truncate: pdf = max(pdf, EPSILON)

```C++
(1.f - max(F))*lambert + cook_torrance
```

一堆噪点啊 （可能是F term的缘故导致PDF和brdf差(1-F)Lambert太大，没有体现重要性）