#include "myglwidget.h"

MyGLWidget::MyGLWidget(QWidget *parent)
	:QOpenGLWidget(parent),
	scene_id(0)
{
}

MyGLWidget::~MyGLWidget()
{

}

/*###################################################
##  函数: initializeGL
##  函数描述： 初始化绘图参数，如视窗大小、背景色等
##  参数描述： 无
#####################################################*/
void MyGLWidget::initializeGL()
{
	glViewport(0, 0, width(), height());  
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_TEXTURE_2D);
    float baseX, baseZ;
    std::fill(std::begin(keyStates), std::end(keyStates), false);
    jumpVelocity = 550.0f; // 例如，设置跳跃的初始速度为20.0f
    moveVelocity = 50.0f;//移动速度
    /*jumpInitialHeight = 1.6f * player->maze->wallsize;
    fallInitialHeight = 1.6f * player->maze->wallsize;*/
    jumpPeakHeight = jumpInitialHeight + 10*player->maze->wallsize;
    isJumping = 0;
    isFalling = 0;
    isforwarding = 0;
    /*player->maze->textures.LoadTexture2D("texture/floor.bmp");  // 路径纹理
    player->maze->textures.LoadTexture2D("texture/wall.bmp");  // 墙壁纹理
    frameSizeForTrap = 2;  // 陷阱的动态纹理数量
    player->maze->textures.LoadTexture2D("texture/dynamic_trap_0.bmp");  
    player->maze->textures.LoadTexture2D("texture/dynamic_trap_1.bmp");
    frameSizeForWall = 8;  // 墙壁的动态纹理数量
    player->maze->textures.LoadTexture2D("texture/wall_1.bmp");
    player->maze->textures.LoadTexture2D("texture/wall_2.bmp");
    player->maze->textures.LoadTexture2D("texture/wall_3.bmp");
    player->maze->textures.LoadTexture2D("texture/wall_4.bmp");
    player->maze->textures.LoadTexture2D("texture/wall_5.bmp");
    player->maze->textures.LoadTexture2D("texture/wall_4.bmp");
    player->maze->textures.LoadTexture2D("texture/wall_3.bmp");
    player->maze->textures.LoadTexture2D("texture/wall_2.bmp");*/
    glDisable(GL_TEXTURE_2D);

    //天空盒
    glEnable(GL_TEXTURE_CUBE_MAP);
    char skyboxFile[6][100] = {
      "skybox_image/1.bmp",
      "skybox_image/2.bmp",
      "skybox_image/top.bmp",
      "skybox_image/bottom.bmp",
      "skybox_image/3.bmp",
      "skybox_image/4.bmp"
    }; 
    skyBox.LoadTextureCube(skyboxFile);
    glDisable(GL_TEXTURE_CUBE_MAP);
    

    /*###########################################################
    ##  功能: Phong 光照模型
    ##  功能描述： 设置物体的材质属性，设置光源，光照模型
    ##  组成部分： 
        环境光（Ambient）：全局环境光设置，影响场景中所有物体的基本颜色。
        漫反射（Diffuse）：光线照射到物体上并在多个方向上散射的效果。
        镜面反射（Specular）：模拟光线照射到物体上并在特定方向上反射的效果。
        衰减（Attenuation）：模拟光线随着距离增加而减弱的效果。
        光源位置（Position）：光源在场景中的位置。
    #############################################################*/

    // 设置物体材质属性
    GLfloat materialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f }; // 环境光属性（红，绿，蓝，Alpha）
    GLfloat materialDiffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f }; // 漫反射属性（红，绿，蓝，Alpha）
    GLfloat materialSpecular[] = { 0.8f, 0.8f, 0.8f, 1.0f }; // 镜面反射属性（红，绿，蓝，Alpha）
    GLfloat shininess[] = { 64.0f }; // 光泽度（数值越高，反射越强烈）
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materialAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materialDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materialSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    // 设置光源的属性
    GLfloat lightSpecular[] = { 0.7f, 0.7f, 0.8f, 1.0f }; // 镜面反射属性（红，绿，蓝，Alpha）
    GLfloat lightDiffuse[] = { 0.6f, 0.6f, 0.9f, 1.0f }; // 漫反射属性（红，绿，蓝，Alpha）
    GLfloat lightAmbient[] = { 0.2f, 0.2f, 0.4f, 1.0f }; // 环境光属性（红，绿，蓝，Alpha）
    GLfloat lightConstAttenuation[] = { 1.0f }; // 光源恒定衰减系数
    GLfloat lightLinearAttenuation[] = { 0.0f }; // 光源线性衰减系数
    GLfloat lightQuadAttenuation[] = { 0.00f }; // 光源二次衰减系数
    //GLfloat lightPosition[] = { 1000.0f, 1300.0f, 1000.0f, 1.0f }; // 光源位置（X，Y，Z，W）W=1代表点光源
    GLfloat LightModelAmbient[] = { 0.6f, 0.6f, 0.6f, 1.0 }; // 全局环境光属性（红，绿，蓝，Alpha）
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular);
    glLightfv(GL_LIGHT0, GL_CONSTANT_ATTENUATION, lightConstAttenuation);
    glLightfv(GL_LIGHT0, GL_LINEAR_ATTENUATION, lightLinearAttenuation);
    glLightfv(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, lightQuadAttenuation);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);


    //Material

    glEnable(GL_DEPTH_TEST);

}

void MyGLWidget::paintGL()
{
	if (scene_id==0) {
		scene_0();
	}
	else {
		scene_1();
	}
}

void MyGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	update();
}

void MyGLWidget::keyPressEvent(QKeyEvent *e) {
	//Press 0 or 1 to switch the scene
	if (e->key() == Qt::Key_0) {
		scene_id = 0;
		update();
	}
	else if (e->key() == Qt::Key_1) {
		scene_id = 1;
		update();
	}
}

void MyGLWidget::scene_0()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, 100.0f, 0.0f, 100.0f, -1000.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(50.0f, 50.0f, 0.0f);
	
	//draw a diagonal "I"
	glPushMatrix();
	glColor3f(0.839f, 0.153f, 0.157f);
	glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
	glTranslatef(-2.5f, -22.5f, 0.0f);
	glBegin(GL_TRIANGLES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(5.0f, 0.0f);
	glVertex2f(0.0f, 45.0f);

	glVertex2f(5.0f, 0.0f);
	glVertex2f(0.0f, 45.0f);
	glVertex2f(5.0f, 45.0f);

	glEnd();
	glPopMatrix();	
}

void MyGLWidget::scene_1()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, width(), 0.0f, height(), -1000.0f, 1000.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.5 * width(), 0.5 * height(), 0.0f);

    //your implementation here, maybe you should write several glBegin
	glPushMatrix();
	//your implementation
	
	glPopMatrix();
}