#pragma once
class MsgSender
{
public:
	static bool IsEnabled();
	static bool Send(void* message, size_t size);
	static bool Patcher(bool state);
};

