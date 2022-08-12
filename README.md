# Monte Carlo Path Tracer

Based on GAMES101 Assignment7

### Goal

:white_check_mark: ​Path Tracing 

:white_check_mark:  Thread pool Acceleration using std::thread

:white_check_mark: Microfacet Material

### Notes

Some self-researched results based on this project (in Chinese)

[Sampling Uniformly on a Manifold I - Rationale](https://zhuanlan.zhihu.com/p/376386495)

[Sampling Uniformly on a Manifold II - Examples](https://zhuanlan.zhihu.com/p/376432029)

or directly see my blog

[Uniformly Sample on a Manifold - 1.Rationale | TaKeTube](https://taketube.github.io/2021/05/30/uniformly-sample-on-manifold-1.html)

[Uniformly Sample on a Manifold - 2.Examples | TaKeTube](https://taketube.github.io/2021/05/30/uniformly-sample-on-manifold-2.html)

### Results (partially)

Run under Intel i5-8300H (4 cores 8 threads)

Check ***results*** folder for more rendered images

**TWO BOXES**

SPP 4096

8 thread 108 min (before optimizing)

![](https://github.com/TaKeTube/GAMES-101/blob/master/Assignment7/Code/results/19.png?raw=true)

---

**BUNNY**

forget the data

![22](https://github.com/TaKeTube/GAMES-101/blob/master/Assignment7/Code/results/22.png?raw=true)

---

**IRON BUNNY**

SPP 8192

212 mins

![40](https://github.com/TaKeTube/GAMES-101/blob/master/Assignment7/Code/results/40.png?raw=true)

---

**PLASTIC BUNNY**

Forget the data

less than 20 mins

![42](https://github.com/TaKeTube/GAMES-101/blob/master/Assignment7/Code/results/42.png?raw=true)

---

**IRON BUNNY WITH GOLD BOX**

SPP 24576

255 mins

![60](https://github.com/TaKeTube/GAMES-101/blob/master/Assignment7/Code/results/60.png?raw=true)

---

**BUNNY WITH GOLD BOX**

SPP 12288

322 mins

![61](https://github.com/TaKeTube/GAMES-101/blob/master/Assignment7/Code/results/61.png?raw=true)

---

**GOLD BUNNY**

SPP 12288

319 mins

![62](https://github.com/TaKeTube/GAMES-101/blob/master/Assignment7/Code/results/62.png?raw=true)