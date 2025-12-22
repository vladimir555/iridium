#!/bin/bash

# Получаем список всех удаленных репозиториев
remotes=$(git remote)

# Определяем текущую ветку
current_branch=$(git branch --show-current)

if [ -z "$current_branch" ]; then
    echo "Ошибка: Не удалось определить текущую ветку. Возможно, вы находитесь в состоянии 'detached HEAD'."
    exit 1
fi

# Выполняем push в каждый удаленный репозиторий
for remote in $remotes; do
    echo "Pushing to $remote/$current_branch..."
    git push $remote $current_branch
done
