/***********************************************************************************************************************************
Archive Push Protocol Handler
***********************************************************************************************************************************/
#include "command/archive/push/file.h"
#include "command/archive/push/protocol.h"
#include "common/debug.h"
#include "common/io/io.h"
#include "common/log.h"
#include "common/memContext.h"
#include "config/config.h"
#include "storage/helper.h"

/***********************************************************************************************************************************
Constants
***********************************************************************************************************************************/
STRING_EXTERN(PROTOCOL_COMMAND_ARCHIVE_PUSH_STR,                     PROTOCOL_COMMAND_ARCHIVE_PUSH);

/***********************************************************************************************************************************
Process protocol requests
***********************************************************************************************************************************/
bool
archivePushProtocol(const String *command, const VariantList *paramList, ProtocolServer *server)
{
    FUNCTION_LOG_BEGIN(logLevelTrace);
        FUNCTION_LOG_PARAM(STRING, command);
        FUNCTION_LOG_PARAM(VARIANT_LIST, paramList);
        FUNCTION_LOG_PARAM(PROTOCOL_SERVER, server);
    FUNCTION_LOG_END();

    ASSERT(command != NULL);

    // Get the repo storage in case it is remote and encryption settings need to be pulled down
    storageRepo();

    // Attempt to satisfy the request -- we may get requests that are meant for other handlers
    bool found = true;

    MEM_CONTEXT_TEMP_BEGIN()
    {
        if (strEq(command, PROTOCOL_COMMAND_ARCHIVE_PUSH_STR))
        {
            protocolServerResponse(
                server,
                varNewStr(
                    archivePushFile(
                        varStr(varLstGet(paramList, 0)), varStr(varLstGet(paramList, 1)),
                        (unsigned int)varUInt64(varLstGet(paramList, 2)), varUInt64(varLstGet(paramList, 3)),
                        varStr(varLstGet(paramList, 4)), (CipherType)varUInt64(varLstGet(paramList, 5)),
                        varStr(varLstGet(paramList, 6)), varBool(varLstGet(paramList, 7)), varIntForce(varLstGet(paramList, 8)))));
        }
        else
            found = false;
    }
    MEM_CONTEXT_TEMP_END();

    FUNCTION_LOG_RETURN(BOOL, found);
}
