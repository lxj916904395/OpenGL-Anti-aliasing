//
//  main.cpp
//  OpenGL-抗锯齿
//
//  Created by zhongding on 2018/12/4.
//  Copyright © 2018 zhongding. All rights reserved.
//

#include "GLTools.h"
#include "GLFrustum.h"

#ifdef __APPLE__
#include <glut/glut.h>
#else
#define FREEGLUT_STATIC
#include <GL/glut.h>
#endif

GLFrustum viewFrustum;
GLShaderManager shaderManager;

GLBatch smallBatch;
GLBatch middBatch;
GLBatch bigBatch;
GLBatch mountainBatch;
GLBatch moonBatch;

#define SMALL_STARS     200
#define MEDIUM_STARS     40
#define LARGE_STARS      15

#define SCREEN_X        800
#define SCREEN_Y        600


void rightKeyction(int value){
    
    switch(value)
    {
        case 1:
            //打开抗锯齿，并给出关于尽可能进行最佳的处理提示
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glEnable(GL_POINT_SMOOTH);
            glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
            glEnable(GL_LINE_SMOOTH);
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
            glEnable(GL_POLYGON_SMOOTH);
            glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
            break;
            
        case 2:
            //关闭抗锯齿
            glDisable(GL_BLEND);
            glDisable(GL_LINE_SMOOTH);
            glDisable(GL_POINT_SMOOTH);
            break;
            
        default:
            break;
    }
    
    glutPostRedisplay();
}

void changeSize(int w, int h){
    if(h == 0)
        h = 1;
    
    glViewport(0, 0, w, h);
    //设置投影模式：正投影
    viewFrustum.SetOrthographic(0.0f, SCREEN_X, 0.0f, SCREEN_Y, -1.0f, 1.0f);
}

void renderScene(void){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    GLfloat vWhite [] = { 1.0f, 1.0f, 1.0f, 1.0f };

   //平面着色器
    shaderManager.UseStockShader(GLT_SHADER_FLAT,viewFrustum.GetProjectionMatrix(),vWhite);
    
     //关闭多重采样
    glDisable(GL_MULTISAMPLE);
    
    //开始画星星
    
    glPointSize(1.2f);
    smallBatch.Draw();
    
    glPointSize(5.f);
    middBatch.Draw();
    
    glPointSize(8.f);
    bigBatch.Draw();
    
    //画山峰
    glLineWidth(3.f);
    mountainBatch.Draw();
    
    //针对多边形开始多重采样
    glEnable(GL_MULTISAMPLE);
    moonBatch.Draw();
    glDisable(GL_MULTISAMPLE);
    
    
    glutSwapBuffers();
}


void setup(){
    
    shaderManager.InitializeStockShaders();
    
    //绘制的200个小星星点
    GLfloat smallVerts[SMALL_STARS][3];
    smallBatch.Begin(GL_POINTS, SMALL_STARS);
    for(int i = 0; i < SMALL_STARS; i++){
        smallVerts[i][0] = (float)(rand() % SCREEN_X);
        smallVerts[i][1] = (float)(rand() % SCREEN_Y);
        smallVerts[i][2] = 0.f;
    }
    smallBatch.CopyVertexData3f(smallVerts);
    smallBatch.End();
    
    //绘制中星星点
    GLfloat middVerts[MEDIUM_STARS][3];
    middBatch.Begin(GL_POINTS, MEDIUM_STARS);
    for(int i = 0; i < MEDIUM_STARS; i++){
        middVerts[i][0] = (float)(rand() % SCREEN_X);
        middVerts[i][1] = (float)(rand() % SCREEN_Y);
        middVerts[i][2] = 0.f;
    }
    middBatch.CopyVertexData3f(middVerts);
    middBatch.End();
    
    //绘制大星星星星点
    GLfloat bigVerts[LARGE_STARS][3];
    bigBatch.Begin(GL_POINTS, LARGE_STARS);
    for(int i = 0; i < LARGE_STARS; i++){
        bigVerts[i][0] = (float)(rand() % SCREEN_X);
        bigVerts[i][1] = (float)(rand() % SCREEN_Y);
        bigVerts[i][2] = 0.f;
    }
    bigBatch.CopyVertexData3f(bigVerts);
    bigBatch.End();
    
    M3DVector3f vMountains[12] = { 0.0f, 25.0f, 0.0f,
        50.0f, 100.0f, 0.0f,
        100.0f, 25.0f, 0.0f,
        225.0f, 125.0f, 0.0f,
        300.0f, 50.0f, 0.0f,
        375.0f, 100.0f, 0.0f,
        460.0f, 25.0f, 0.0f,
        525.0f, 100.0f, 0.0f,
        600.0f, 20.0f, 0.0f,
        675.0f, 70.0f, 0.0f,
        750.0f, 25.0f, 0.0f,
        800.0f, 90.0f, 0.0f };
    
    mountainBatch.Begin(GL_LINE_STRIP, 12);
    mountainBatch.CopyVertexData3f(vMountains);
    mountainBatch.End();
    
    
    //月亮
    GLfloat x = 700.0f;
    GLfloat y = 500.0f;
    GLfloat r = 50.0f;
    GLfloat angle = 0.0f;
    
    M3DVector3f vVerts[SMALL_STARS];
    moonBatch.Begin(GL_TRIANGLE_FAN, 34);
    int nVerts = 0;
    vVerts[nVerts][0] = x;
    vVerts[nVerts][1] = y;
    vVerts[nVerts][2] = 0.0f;
    for(angle = 0; angle < 2.0f * 3.141592f; angle += 0.2f) {
        nVerts++;
        vVerts[nVerts][0] = x + float(cos(angle)) * r;
        vVerts[nVerts][1] = y + float(sin(angle)) * r;
        vVerts[nVerts][2] = 0.0f;
    }
    nVerts++;
    
    vVerts[nVerts][0] = x + r;;
    vVerts[nVerts][1] = y;
    vVerts[nVerts][2] = 0.0f;
    moonBatch.CopyVertexData3f(vVerts);
    moonBatch.End();
    
}


int main(int argc , char* argv[]){
    
    glutInit(&argc, argv);
    //GLUT_MULTISAMPLE：开启多重采样
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_MULTISAMPLE);
    glutInitWindowSize(800, 600);
    glutCreateWindow("抗锯齿");
    
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    
    glutCreateMenu(rightKeyction);
    glutAddMenuEntry("打开颜色混合抗锯齿", 1);
    glutAddMenuEntry("关闭颜色混合抗锯齿", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    
    GLenum err = glewInit();
    
    if(GLEW_OK != err){
        return 1;
    }
    
    setup();
    glutMainLoop();
    
    return 0;
}
