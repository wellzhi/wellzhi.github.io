#!/bin/bash

set -e

echo "Generating dynamic pipeline for ${CI_COMMIT_REF_NAME}"

# 确定 GROUP 和 SUB_ENV 基于分支名称
if [[ "$CI_COMMIT_REF_NAME" =~ ^variant/(.+)$ ]]; then
  SUB_ENV=test
  GROUP="${BASH_REMATCH[1]}"
elif [[ "$CI_COMMIT_REF_NAME" =~ ^release/(.+)$ ]]; then
  SUB_ENV=prod
  GROUP="${BASH_REMATCH[1]}"
else
  echo "Unrecognized branch: $CI_COMMIT_REF_NAME"
  exit 1
fi

echo "Detected GROUP: $GROUP, SUB_ENV: $SUB_ENV"

# 使用 yq 获取对应 GROUP 下的所有 ENVS
ENVS=$(yq e ".${GROUP} | keys | .[]" environments.yml)

echo "Detected ENVS: $ENVS, SUB_ENV: $SUB_ENV"

# 设置 VERSION
# VERSION 已在 variables 中定义
echo "VERSION: $VERSION"

# 开始构建 child-ci.yml 文件
cat <<EOL > child-ci.yml
stages:
  - deploy-child

EOL

# 遍历每个 ENV，生成对应的部署任务
for ENV in $ENVS; do
  # 获取 context_path
  CONTEXT_PATH=$(yq e ".${GROUP}.${ENV}.context_path" environments.yml)

  # 使用不同的分隔符（例如 #）来避免与路径中的 / 冲突
  cat <<EOL >> child-ci.yml
deploy_${ENV}_${SUB_ENV}:
  stage: deploy-child
  image:
    name: 172.22.121.50/dockerio/bitnami/kubectl:1.23
    entrypoint: [""]
  tags:
    - "k8s_runner_persionnel_matching"
  rules:
    - if: '\$CI_COMMIT_REF_NAME =~ /^variant\/.+$/'
      when: on_success
    - if: '\$CI_COMMIT_REF_NAME =~ /^release\/.+$/'
      when: manual
    - when: never
  script:
    - echo "Deploying ${REPO_NAME} to ${ENV}/${SUB_ENV} with CONTEXT_PATH=${CONTEXT_PATH} APP=${ENV}"
    - sed -i 's#{{REPO_NAME}}#${REPO_NAME}#g' $PROJECT_PATH/deployment.yaml
    - sed -i 's#{{ENV}}#${ENV}#g' $PROJECT_PATH/deployment.yaml
    - sed -i 's#{{SUB_ENV}}#${SUB_ENV}#g' $PROJECT_PATH/deployment.yaml
    - sed -i 's#{{CONTEXT_PATH}}#${CONTEXT_PATH}#g' $PROJECT_PATH/deployment.yaml
    - sed -i 's#{{VERSION}}#${VERSION}#g' $PROJECT_PATH/deployment.yaml
    - kubectl apply -f $PROJECT_PATH/deployment.yaml
  allow_failure: true

EOL

done

echo "child-ci.yml generated successfully."
