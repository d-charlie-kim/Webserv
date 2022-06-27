#include "Client.hpp"

void	delete_function(Request &request, Client &client)
{
    /*
        NOTE delete 메서드 처리 후 어떤 응답보낼지 선택가능해보입니다 그냥 204로만 해도 될까요
        
            응 답
        DELETE 메서드를 성공적으로 적용한 후에 사용할 수 있는 응답 상태 코드는 다음과 같이 몇 가지가 있습니다.

        아마도 명령을 성공적으로 수행할 것 같으나 아직은 실행하지 않은 경우 202 (Accepted) 상태 코드.
        명령을 수행했고 더 이상 제공할 정보가 없는 경우 204 (No Content) 상태 코드.
        명령을 수행했고 응답 메시지가 이후의 상태를 설명하는 경우 200 (OK) 상태 코드.
    */


    // 존재하지 않는 경로라면 404
	struct stat		status;
	if (stat(request.path.c_str(), &status))
	{
		request.status_code = 404;
		return ;
	}
	
	// 디렉토리면 405
	if (request.is_directory)
	{
		request.status_code = 405;
		return ;
	}
    // TODO 특정 경로만 허용 다른 경로면 405
    // POST 이미지 업로드 요청으로 저장해둔 폴더만 DELETE 요청이 가능하게
}