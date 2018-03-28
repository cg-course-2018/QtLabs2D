#include "SimpleScene.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec2.hpp>
#include "RandomColorGenerator.h"
#include <iostream>

/*SimpleScene::SimpleScene()
{
}*/
namespace
{

	// Этот вершинный шейдер выполняет две задачи:
	//  1) передаёт цвет фрагментному шейдеру
	//  2) выполняет ортографическое проецирование вершины,
	//     по сути проецируя вершину на плоскость экрана.
	const char kVertexShaderCode[] = R"**(#version 130
in vec2 i_position;
in vec4 i_color;
out vec4 v_color;
uniform mat4 u_projection_matrix;
void main()
{
    v_color = i_color;
    gl_Position = u_projection_matrix * vec4( i_position, 0.0, 1.0 );
}
)**";

	// Этот фрагментный шейдер устанавливает фрагменту переданный из
	//  вершинного шейдера цвет.
	static const char kFragmentShaderCode[] = R"**(#version 130
in vec4 v_color;
void main()
{
    gl_FragColor = v_color;
}
)**";


	glm::vec2 euclidean(float radius, float angle)
	{
		return { radius * cos(angle), radius * sin(angle) };
	}

	constexpr float PI = 3.1415926f;

	std::vector<VertexP2C4> tesselateConvexByCenter(const glm::vec2& center, const std::vector<glm::vec2>& hullPoints, RandomColorGenerator& colorGen)
	{
		const size_t size = hullPoints.size();
		std::vector<VertexP2C4> verticies;
		verticies.reserve(3u * size);
		for (size_t pointIndex = 0; pointIndex < size; ++pointIndex)
		{
			// Генерируем цвет треугольника и добавляем три его вершины в список.
			const glm::vec4 triangleColor = colorGen.GenerateColor();
			const size_t nextPointIndex = (pointIndex + 1) % size;
			verticies.push_back({ hullPoints.at(pointIndex), triangleColor });
			verticies.push_back({ hullPoints.at(nextPointIndex), triangleColor });
			verticies.push_back({ center, triangleColor });
		}

		return verticies;
	}

	// Функция делит круг на треугольники,
	//  возвращает массив с вершинами треугольников.
	std::vector<VertexP2C4> tesselateCircle(float radius, const glm::vec2& center, RandomColorGenerator& colorGen)
	{
		assert(radius > 0);

		// Круг аппроксимируется с помощью треугольников.
		// Внешняя сторона каждого треугольника имеет длину 2.
		constexpr float step = 2;
		// Число треугольников равно длине окружности, делённой на шаг по окружности.
		const auto pointCount = static_cast<unsigned>(radius * 2 * PI / step);

		// Вычисляем точки-разделители на окружности.
		std::vector<glm::vec2> points(pointCount);
		for (unsigned pi = 0; pi < pointCount; ++pi)
		{
			const auto angleRadians = static_cast<float>(2.f * PI * pi / pointCount);
			points[pi] = center + euclidean(radius, angleRadians);
		}

		return tesselateConvexByCenter(center, points, colorGen);
	}



	// Генерирует список вершин треугольников для выпуклого многоугольника, заданного вершинами.
	std::vector<VertexP2C4> tesselateConvex(const std::vector<glm::vec2>& verticies, RandomColorGenerator& colorGen)
	{
		// Центр выпуклого многоугольника - это среднее арифметическое его вершин
		const glm::vec2 center = std::accumulate(verticies.begin(), verticies.end(), glm::vec2()) / float(verticies.size());
		return tesselateConvexByCenter(center, verticies, colorGen);
	}
} // anonymous namespace

void SimpleScene::initializeShaders()
{
	m_vertexShader = compileShader(GL_VERTEX_SHADER, kVertexShaderCode);
	m_fragmentShader = compileShader(GL_FRAGMENT_SHADER, kFragmentShaderCode);
	m_program = linkProgram({ m_vertexShader, m_fragmentShader });
}

void SimpleScene::initialize()
{
	QOpenGLFunctions_3_3_Core::initializeOpenGLFunctions();

	initializeShaders();

	//  в этот буфер мы запишем параметры вершин для видеокарты.
	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

	// Создаём Vertex Array Object (VAO), который хранит связи между данными
	//  в VBO и переменными шейдера.
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
}

void SimpleScene::update(float deltaSeconds)
{
	(void)deltaSeconds;
}

void SimpleScene::redraw(unsigned width, unsigned height)
{
	(void)width;
	(void)height;
}
GLuint SimpleScene::compileShader(GLenum type, const std::string & source)
{
	// Выделяем ресурс шейдера
	GLuint shader = glCreateShader(type);

	// Передаём исходный код шейдера видеодрайверу
	const auto length = static_cast<int>(source.length());
	const char* sourceLine = source.data();
	glShaderSource(shader, 1, (const GLchar**)&sourceLine, &length);

	// Просим видеодрайвер скомпилировать шейдер и проверяем статус
	glCompileShader(shader);

	GLint ok = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
	if (ok == GL_FALSE)
	{
		// При неудаче есть лог ошибок, который мы соберём
		// и в первую очередь надо узнать длину лога.
		GLint logLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		// Зная длину, выделяем строку нужного размера и копируем в неё лог
		std::string log(logLength, '\0');
		GLsizei ignored = 0;
		glGetShaderInfoLog(shader, log.size(), &ignored, (GLchar*)log.data());

		// Бросаем исключение, прикрепив к нему лог
		throw std::runtime_error("shader compilation failed: " + log);
	}

	return shader;
}
GLuint SimpleScene::linkProgram(const std::vector<GLuint>& shaders)
{
	// Запрашиваем у видеодрайера новый объект.
	GLuint obj = glCreateProgram();

	// Прикрепляем ранее скомпилированные шейдеры.
	for (GLuint shader : shaders)
	{
		glAttachShader(obj, shader);
	}

	// Просим видеодрайвер выполнить компоновку и проверяем статус.
	glLinkProgram(obj);

	GLint status = 0;
	glGetProgramiv(obj, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		// При неудаче есть лог ошибок, который мы соберём
		// и в первую очередь надо узнать длину лога.
		GLint logLength = 0;
		glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &logLength);

		// Зная длину, выделяем строку нужного размера и копируем в неё лог
		std::string log(logLength, '\0');
		GLsizei ignored = 0;
		glGetProgramInfoLog(obj, log.size(), &ignored, (GLchar*)log.data());

		// Бросаем исключение, прикрепив к нему лог
		throw std::runtime_error("program linking failed " + log);
	}

	return obj;
}
SimpleScene::~SimpleScene()
{
	glDeleteProgram(m_program);
	glDeleteProgram(m_vertexShader);
	glDeleteProgram(m_fragmentShader);
	glDeleteBuffers(1, &m_vbo);
	glDeleteVertexArrays(1, &m_vbo);
}
void SimpleScene::bindVertexData(const std::vector<VertexP2C4> &verticies)
{
	// OpenGL должен получить байтовые смещения полей относительно структуры VertexP2C4.
	const void *colorOffset = reinterpret_cast<void *>(offsetof(VertexP2C4, rgba));
	const void *posOffset = reinterpret_cast<void *>(offsetof(VertexP2C4, xy));
	const size_t stride = sizeof(VertexP2C4);

	// Привязываем атрибут i_color к данным в вершинном буфере.
	const int colorLocation = glGetAttribLocation(m_program, "i_color");
	glEnableVertexAttribArray(colorLocation);
	glVertexAttribPointer(colorLocation, glm::vec4().length(), GL_FLOAT, GL_FALSE, stride, colorOffset);

	// Привязываем атрибут i_position к данным в вершинном буфере.
	const int posLocation = glGetAttribLocation(m_program, "i_position");
	glEnableVertexAttribArray(posLocation);
	glVertexAttribPointer(posLocation, glm::vec2().length(), GL_FLOAT, GL_FALSE, stride, posOffset);

	// Загружаем данные в вершинный буфер.
	glBufferData(GL_ARRAY_BUFFER, stride * verticies.size(), verticies.data(), GL_STATIC_DRAW);
}
