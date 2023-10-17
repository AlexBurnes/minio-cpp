#include "client.h"
#include "http.h"

int main(int argc, char* argv[]) {

    minio::s3::BaseUrl base_url("http://127.0.0.1:9000", false);
    minio::creds::StaticProvider provider(
      "test", "miniostorage");
    minio::s3::Client client(base_url, &provider);

    // Make bucket 'upload-big-object'
    minio::s3::MakeBucketArgs bucket_args;
    bucket_args.bucket = "test-upload-big-object";
    minio::s3::MakeBucketResponse bucket_resp = client.MakeBucket(bucket_args);
    if (!bucket_resp) {
        std::cout << "unable to create bucket; " << bucket_resp.Error().String() << std::endl;
        return EXIT_FAILURE;
    }

    // Set bucket versioning
    minio::s3::SetBucketVersioningArgs versioning_args;
    versioning_args.bucket = bucket_args.bucket;
    versioning_args.status = true;
    minio::s3::SetBucketVersioningResponse versioning_resp = client.SetBucketVersioning(versioning_args);
    if (!versioning_resp) {
        std::cout << "unable to set bucket versioning; " << versioning_resp.Error().String()
                  << std::endl;
        return EXIT_FAILURE;
    }

    // Upload object
    minio::s3::UploadObjectArgs upload_args;
    upload_args.bucket = bucket_args.bucket;
    upload_args.object = "vcpkg-master.zip";
    upload_args.filename = "./vcpkg-master.zip";
    minio::s3::UploadObjectResponse upload_resp = client.UploadObject(upload_args);
    if (!upload_resp) {
        std::cout << "unable upload object  '" << upload_args.object << "' , error "
                  << upload_resp.Error().String() << std::endl;
    }
    std::cout << "'" << upload_args.filename << "' is successfully uploaded as "
              << "object '" << upload_args.object << "' "
              << "version '" <<  upload_resp.version_id << "' "
              << "etag '" <<  upload_resp.etag << "'."
              << std::endl;

    std::cout << "x-amz-version-id: " << upload_resp.headers.GetFront("x-amz-version-id") << std::endl;

    // Remove object
    minio::s3::RemoveObjectArgs remove_args;
    remove_args.bucket = bucket_args.bucket;
    remove_args.object = upload_args.object;
    if (upload_resp.version_id != "") {
        remove_args.version_id = upload_resp.version_id;
    }
    minio::s3::RemoveObjectResponse remove_resp = client.RemoveObject(remove_args);
    if (!remove_resp) {
        std::cout << "failed remove object  '" << upload_args.object << "' , error "
                  << remove_resp.Error().String() << std::endl;
    }

    // Delete bucket
    minio::s3::RemoveBucketArgs delete_args;
    delete_args.bucket = bucket_args.bucket;
    minio::s3::RemoveBucketResponse delete_resp = client.RemoveBucket(delete_args);
    if (!delete_resp) {
        std::cout << "failed delete bucket '" << delete_args.bucket << "' , error "
                  << delete_resp.Error().String() << std::endl;
    }

    return upload_resp.version_id == "" 
        ? EXIT_FAILURE
        : EXIT_SUCCESS;
}
