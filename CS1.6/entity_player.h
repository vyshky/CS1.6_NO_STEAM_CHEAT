class EntityPlayer {
public:
	EntityPlayer(uintptr_t hw_dll = 0) {
		this->hw_dll = hw_dll;
		base_player_list = hw_dll + 0x1AFBFC;
	}
	uintptr_t get_player_size() {
		return PLAYER_SIZE;
	}
	uintptr_t get_array_size() const { return ARRAY_SIZE; }
	static constexpr uintptr_t PLAYER_SIZE = 0xBB8;      // 3000 байт
	static constexpr uintptr_t ARRAY_SIZE = 0x182B8;     // 99000 байт
	static constexpr uintptr_t OFFSET_X = 0xB48;
	static constexpr uintptr_t OFFSET_Y = 0xB4C;
	static constexpr uintptr_t OFFSET_Z = 0xB50;
	static constexpr uintptr_t OFFSET_PLAYER_MODEL = 0xB94;
private:
	uintptr_t iterator = 0;
	uintptr_t hw_dll = 0;
	uintptr_t base_player_list = 0;
};