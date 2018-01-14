namespace vega {
  namespace dicom {
    template <typename Iter>
    bool RawReader::read_into(Iter begin, Iter end) {
      for(auto it = begin; it != end; ++it) {
        if (!this->read_into(&(*it))) { return false; }
      }
      return true;
    }

    template <typename T>
    bool RawReader::read_into(T* ptr) {
      if (m_state.swap()) {
        // Have to swap the bytes to fix endian mis-match
        char* raw = reinterpret_cast<char*>(ptr);
        for (size_t j = 0; j < sizeof(T); ++j) {
          m_is->read(raw + sizeof(T)-j-1, 1);
          if (m_is->gcount() == 0) return false;
        }
      }
      else {
        // Read in bytes directly
        m_is->read(reinterpret_cast<char*>(ptr), sizeof(T));
        if ((unsigned)m_is->gcount() < sizeof(T)) return false;
      }

      return true;
    }
  }
}
