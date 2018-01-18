#include "HttpServer.h"

HttpServer::HttpServer(int port) : port(port)
{
}

void HttpServer::start()
{
    daemon = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY, port,
                              NULL, NULL,
                              &_handleRequest, this,
                              MHD_OPTION_NOTIFY_COMPLETED,
                              &_handleRequestComplete, this,
                              MHD_OPTION_END);
}

void HttpServer::stop()
{
    MHD_stop_daemon(daemon);
}

void HttpServer::process()
{
}

void HttpServer::registerRoute(HttpMethod method, string url, RequestHandler handler)
{
    switch (method)
    {
    case METHOD_GET:
        getRoutes[url] = handler;
        break;
    case METHOD_POST:
        postRoutes[url] = handler;
        break;
    case METHOD_PUT:
        putRoutes[url] = handler;
        break;
    case METHOD_DELETE:
        deleteRoutes[url] = handler;
        break;
    }
}

int HttpServer::handleRequest(Connection connection,
                              string url, string method, string version,
                              const char *uploadData, size_t *uploadDataSize,
                              ConnectionInfo **conInfoPtr)
{
    if (*conInfoPtr == NULL)
    {
        ConnectionInfo *conInfo = new ConnectionInfo();
        conInfo->answerString = NULL;
        if (method == "POST")
        {
            conInfo->postProcessor = MHD_create_post_processor(connection, PostBufferSize,
                                                               _iteratePost, (void *)conInfo);

            if (NULL == conInfo->postProcessor)
            {
                free(conInfo);
                return MHD_NO;
            }
            conInfo->method = METHOD_POST;
        }
        else
            conInfo->method = METHOD_GET;
        *conInfoPtr = conInfo;
        return MHD_YES;
    }
    if (method == "GET")
    {
        try
        {
            return getRoutes.at(url)(connection, (json *)NULL);
        }
        catch (out_of_range e)
        {
            return MHD_NO;
        }
    }
    else if (method == "POST")
    {
        try
        {
            return postRoutes.at(url)(connection, (json *)NULL);
        }
        catch (out_of_range e)
        {
            return MHD_NO;
        }
    }
    else if (method == "PUT")
    {
        try
        {
            return putRoutes.at(url)(connection, (json *)NULL);
        }
        catch (out_of_range e)
        {
            return MHD_NO;
        }
    }
    else if (method == "DELETE")
    {
        try
        {
            return deleteRoutes.at(url)(connection, (json *)NULL);
        }
        catch (out_of_range e)
        {
            return MHD_NO;
        }
    }
    // if (0 == strcmp(method, "POST"))
    // {
    //     struct connection_info_struct *con_info = *con_cls;

    //     if (*upload_data_size != 0)
    //     {
    //         MHD_post_process(con_info->postprocessor, upload_data,
    //                          *upload_data_size);
    //         *upload_data_size = 0;

    //         return MHD_YES;
    //     }
    //     else if (NULL != con_info->answerstring)
    //         return send_page(connection, con_info->answerstring);
    // }
    // return send_page(connection, errorpage);
    return MHD_NO;
}

int HttpServer::sendResponse(Connection connection, json &j)
{
    int ret;
    char responseBuffer[ResponseBufferSize];
    MHD_Response *response;
    strncpy(responseBuffer, j.dump(4).c_str(), ResponseBufferSize);
    response = MHD_create_response_from_buffer(strlen(responseBuffer),
                                               (void *)responseBuffer, MHD_RESPMEM_MUST_COPY);

    MHD_add_response_header(response, "Content-Type", "application/json");

    ret = MHD_queue_response(connection, MHD_HTTP_OK, response);
    MHD_destroy_response(response);
    return ret;
}

int HttpServer::_handleRequest(void *cls, Connection connection,
                               const char *url, const char *method, const char *version,
                               const char *uploadData, size_t *uploadDataSize,
                               void **conCls)
{
    return ((HttpServer *)cls)->handleRequest(connection, url, method, version, uploadData, uploadDataSize, (ConnectionInfo **)conCls);
}

void HttpServer::_handleRequestComplete(void *cls, Connection connection,
                                        void **conCls, enum MHD_RequestTerminationCode toe)
{
    ConnectionInfo *conInfo = (ConnectionInfo *)*conCls;

    if (NULL == conInfo)
        return;
    if (conInfo->method == METHOD_POST)
    {
        MHD_destroy_post_processor(conInfo->postProcessor);
        if (conInfo->answerString)
            free(conInfo->answerString);
    }

    free(conInfo);
    *conCls = NULL;
}

int HttpServer::_iteratePost(void *coninfoCls,
                             enum MHD_ValueKind kind, const char *key,
                             const char *filename, const char *contentType,
                             const char *transferEncoding, const char *data,
                             uint64_t off, size_t size)
{
}