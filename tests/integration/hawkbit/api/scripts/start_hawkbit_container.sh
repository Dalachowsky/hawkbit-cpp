
docker run \
    --env SPRING_DATASOURCE_URL="jdbc:h2:/app/artifactrepo/database;MODE=LEGACY" \
    -p 8080:8080 \
    --mount type=bind,src=./testdata,dst=/app/artifactrepo \
    hawkbit/hawkbit-update-server:1.1.0