#pragma once
class MsgSender final
{
public:
	static bool IsEnabled();
	static bool Send(void* message, size_t size);
	static bool Patcher(bool state);

private:
	virtual void __dummy() = 0;
};

