#include "TTYDevice.h"
#include "KeyboardDevice.h"

size_t TTYDevice::_current_tty;
TTYDevice *TTYDevice::_ttys[];

TTYDevice *TTYDevice::current_tty()
{
	return _ttys[_current_tty];
}

void TTYDevice::set_current_tty(size_t tty)
{
	TTYDevice *current = _ttys[_current_tty];
	if (current != nullptr) current->_active = false;
	_current_tty = tty;
	KeyboardDevice::inst()->set_handler(_ttys[_current_tty]);
}

void TTYDevice::register_tty(size_t id, TTYDevice *device)
{
	if (_ttys[id]) PANIC("DUPLICATE_TTY", "A TTY tried to register with an ID that was already registered.", true);
	_ttys[id] = device;
}


TTYDevice::TTYDevice(size_t id, const DC::string& name, unsigned int major, unsigned int minor) : CharacterDevice(major, minor), _name(name), _input_buffer(1024), _id(id)
{
	register_tty(id, this);
}

ssize_t TTYDevice::write(FileDescriptor &fd, size_t offset, const uint8_t *buffer, size_t count)
{
	for (auto i = 0; i < count; i++) putch(buffer[i]);
	return count;
}

ssize_t TTYDevice::read(FileDescriptor &fd, size_t offset, uint8_t *buffer, size_t count)
{
	count = min(count, _input_buffer.size());
	size_t count_loop = count;
	while (count_loop--) *buffer++ = _input_buffer.pop();
	return count;
}

void TTYDevice::set_active()
{
	set_current_tty(_id);
}

bool TTYDevice::active()
{
	return _active;
}

void TTYDevice::handle_key(KeyEvent event)
{
	if (!event.pressed()) return;
	if (!event.character) return;
	_input_buffer.push(event.character);
	putch(event.character);
}