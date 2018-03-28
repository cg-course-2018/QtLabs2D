#pragma once

namespace glcore
{
// Бросает исключение, если ранее произошла ошибка при выполнении одной из
//  операций OpenGL. В этом случае видеодрайвере установлен флаг ошибки.
void validateOpenGLError();

// Инициализирует библиотеку glbinding.
void initGLBinding();

} // namespace glcore
