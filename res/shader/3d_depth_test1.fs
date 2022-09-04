#version 330 core
precision highp float;
out vec4 FragColor;
void main(){
    //gl_FragCoord的z值即最后存储在深度缓存中的z值，范围是[0,1]
    //把视锥体世界坐标系中的z值，转化为深度缓存中的z值，即[near,far]转为[0,1]
    //实际采用的是非线性转化
    //z_in_buffer=(1/z_in_world - 1/near)/(1/far-1/near)
    //所以在很靠近近平面的区间，其z值转化已经占了[0,1]区间的绝大部分
    //即较小的z值，转化后，处于[0,1]区间的绝大靠前部分
    //当z值稍微大一点，转化后，就已经非常接近于1了
    //所以这里使用转化后的z值来显示颜色，因为大部分z值都会比较接近1，所以这里主要会是白色
    float z=gl_FragCoord.z;
    if(z>=1.0f)
        FragColor=vec4(vec3(1,0,0),1);
    else if(z>=0.95f && z<1.0f)
        FragColor=vec4(vec3(0,1,0),1);
    else
        FragColor=vec4(vec3(z),1);
}
