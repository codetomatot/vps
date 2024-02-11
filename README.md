C malloc be like:
![image](https://github.com/codetomatot/vps/assets/75332544/e32f4826-b10c-4f4b-a7bf-e4d92ef2554a)


pi sources (not 40 digits that is acutally bad):
https://www.jpl.nasa.gov/edu/news/2016/3/16/how-many-decimals-of-pi-do-we-really-need/

Math proofs and derivations here:

angle between absolute x-axis and any vector
```math
\vec{v_1} = \begin{bmatrix}
x \\ y
\end{bmatrix} \\
\vec{v_2} = \begin{bmatrix}
x \\ 0
\end{bmatrix} \\
\cos(\phi) = \frac{\vec{v_1} \cdot \vec{v_2}}{\| \vec{v_1} \| \| \vec{v_2} \|} \\
\implies \cos(\phi) = \frac{x^2}{\sqrt{x^2 + y^2}\sqrt{x^2}} \implies \phi = \arccos \left(\frac{x}{\sqrt{x^2+y^2}} \right)

```

the rotational matrix:
say we have a normal 2d coordinate system,and some arbitrary vector A  
```math
A_x = A\cos(\theta)
A_y = A\sin(\theta)
```
after a rotation of phi radians of the entire coordinate system:
```math
A_x' = A\cos(\theta - \phi) = A\cos(\theta)\cos(\phi) + A\sin(\theta)\sin(\phi) = A_x\cos(\phi) + A_y\sin(\phi)
A_y' = A\sin(\theta - \phi) = A\sin(\theta)\cos(\phi) - A\cos(\theta)\sin(\phi) = A_y\cos(\phi) - A_x\sin(\phi)
```
therefore in matrix:
```math
\begin{bmatrix}
A_x' \\ A_y'
\end{bmatrix}
=
\begin{bmatrix}
\cos(\phi) & \sin(\phi) \\
-\sin(\phi) & \cos(\phi)
\end{bmatrix}
\begin{bmatrix}
A_x \\ A_y
\end{bmatrix}
```
