# Описание

Игра Endless Survival представляет собой бесконечную 2д survival игру с видом
сверху. В ней вам предстоит сразиться с тысячами монстров, продержаться как
можно дольше. Усильте своего персонажа, чтобы выдержать новые, более
сложные волны врагов. Найдите Звезду Эдема в бесконечном мире, чтобы
навсегда искоренить зло

# Игровой процесс

Игрок исследует открытый мир, взаимодействует с объектами и сражается с врагами, такими как Бегун, Прыгун и Босс. Для борьбы улучшайте свое оружие, чтобы одолеть любого врага.

# Цель Игры

Главная цель игры — Найти звезду Эдема.

# Видео геймплея

[Смотреть видео геймплея](https://drive.google.com/file/d/1C4JgXZqDNDg44YZvI_N_0jWcBCTyWuN9/view?usp=sharing)

# Инструкция по сборке и установке игры

1. Склонируйте репозиторий проекта:

   ```sh
   git clone https://github.com/foblako/EndlessSurvivors.git
   ```
![Вывод должен выглядеть примерно так:](image.png)

2. Перейдите в директорию проекта:

   ```sh
   cd EndlessSurvivors
   ```

3. Создайте директорию сборки и перейдите в неё:

   ```sh
   mkdir build && cd build
   ```
![В результате мы в папке проекта, осталось собрать игру](image-1.png)

4. Запустите CMake для генерации файлов сборки:

   ```sh
   cmake -G "MinGW Makefiles" ..
   ```

   ![Отлично, проект сконфигурирован](image-2.png)

5. Соберите проект с помощью Make:

   ```sh
   mingw32-make
   ```

   ![Успешная сборка игры должна дойти до 100% и завершиться](image-3.png)

6. Запустите игру:

   ```sh
   cd bin && main.exe
   ```