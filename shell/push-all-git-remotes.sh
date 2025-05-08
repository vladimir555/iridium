#!/bin/bash

# Получаем список всех удаленных репозиториев
remotes=$(git remote)

# Выполняем push в каждый удаленный репозиторий
for remote in $remotes; do
    echo "Pushing to $remote..."
    git push $remote develop
done
