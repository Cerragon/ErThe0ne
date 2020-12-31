#pragma once
class MsgSender final
{
public:
	static bool IsEnabled();
	static bool Send(void* message, size_t size);
	static bool Patcher(bool enabled);

private:
	virtual void Dummy() = 0;
};
