C malloc be like:
![image](https://github.com/codetomatot/vps/assets/75332544/e32f4826-b10c-4f4b-a7bf-e4d92ef2554a)


pi sources (not 40 digits that is acutally bad):
https://www.jpl.nasa.gov/edu/news/2016/3/16/how-many-decimals-of-pi-do-we-really-need/

https://math.libretexts.org/Bookshelves/Applied_Mathematics/Mathematics_for_Game_Developers_(Burzynski)/04%3A_Matrices/4.04%3A_Rotation_Matrices_in_2-Dimensions

once more trying to fix the rotations:
![image](https://github.com/codetomatot/vps/assets/75332544/86d96864-aa20-4f14-930d-34c54abfef14)


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
![image](https://github.com/codetomatot/vps/assets/75332544/343ec17c-39df-4797-9f5b-7aaa1a7c5334)



using cross product to determine clockwise or counterclockwise rotations e.g negative cross product is clockwise whereas positive is counterclockwise
```math
\det\left| \begin{bmatrix}
\hat{i} & \hat{j} & \hat{k} \\
x & y & 0 \\
1 & 0 & 0
\end{bmatrix} \right|
\implies -y
```
