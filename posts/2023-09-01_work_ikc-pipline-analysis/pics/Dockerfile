# Dockerfile

FROM 172.22.121.50/gzhu/library/openjdk:8 as builder

ENV BASE_DIR="/home"

WORKDIR /$BASE_DIR

ADD ./agileboot-admin/target/*.jar ./app.jar

RUN java -Djarmode=layertools -jar app.jar extract

FROM 172.22.121.50/gzhu/library/openjdk:8-slim

ENV BASE_DIR="/home"

WORKDIR /$BASE_DIR

RUN apt-get update && apt-get install -y fontconfig libfreetype6 && apt-get install -y fonts-wqy-zenhei
RUN ln -sf /usr/share/zoneinfo/Asia/Shanghai /etc/localtime && echo 'Asia/Shanghai' >/etc/timezone

COPY --from=builder $BASE_DIR/dependencies/ ./
COPY --from=builder $BASE_DIR/spring-boot-loader/ ./
COPY --from=builder $BASE_DIR/snapshot-dependencies/ ./
COPY --from=builder $BASE_DIR/application/ ./

EXPOSE 10010

# 使用占位符替换 Spring profiles
ENTRYPOINT ["java", "-Dspring.profiles.active=basic,base,extended", "org.springframework.boot.loader.JarLauncher"]
