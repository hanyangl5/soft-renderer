# soft renderer

soft renderer是一个C++的不依赖任何图形API的软渲染器，实现了基本的渲染管线，如几何变换，背面剔除，光栅化，着色等功能

--- 

####  使用方法: 

移动到项目的根目录下

~~~
mkdir build && cd build
cmake .. -G"Unix Makefiles"
make
./Rasterizer.exe
~~~

---

#### 特性
- 线框渲染 
- 光栅化
- 纹理映射
- 基础着色
- 第一人称摄像机
- FPS Counting

---

#### Gallery

- 线框渲染
  
<img src="test/figures/wireframe/cube.png" height="200" width="200">
<img src="test/figures/wireframe/teapot.png" height="200" width="200">

- 光栅化 

<img src="test/figures/rasterize/triangle.png" height="200" width="200">
<img src="test/figures/rasterize/cube.png" height="200" width="200">

- 纹理映射 

<img src="test/figures/texture/texture.png" height="200" width="200">
<img src="test/figures/texture/rock.png" height="200" width="200">

- 其他

<img src="test/figures/scene.png" height="200" width="200">
<img src="test/figures/bunny.png" height="200" width="200">