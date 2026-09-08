
# Preparing a test DB

Test DB was prepared by running:

```sh
docker run \
    --env SPRING_DATASOURCE_URL="jdbc:h2:/app/artifactrepo/datab;MODE=LEGACY" \
    -p 8080:8080 \
    --mount type=bind,src=./data,dst=/app/artifactrepo \
    hawkbit/hawkbit-update-server:1.1.0
```

After starting the container, changes to DB were made.

# Description of the test DB

Test DB contains the following

- Targets of type `example`  TODO
  - `target_const` - for tests which will not modify the target
  - `target_new_version_assigned_const` - for tests TODO
- Distribution set type `test_distribution_set`
  - used by target type `example`
  - with mandatory module `test_firmware`
- Software module type `test_firmware`
  -  with versions `1.0.0` and `2.0.0`
