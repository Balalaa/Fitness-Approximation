#-*- coding: utf-8 -*-


class Dataset:
    """ 데이터셋 클래스 """

    def __init__(self):
        """ 데이터셋 생성자 """
        self._idx = 0
        self.num_examples = 0
        self.features = list()
        self.labels = list()

    def __del__(self):
        """ 데이터셋 소멸자 """
        del self._idx
        del self.num_examples
        del self.features
        del self.labels

    def reset_batch(self):
        """ 배치 인덱스를 초기화 """
        self._idx = 0

    def next_batch(self, batch_size):
        """
        배치 크기만큼 다음 배치를 반환

        :param batch_size: int
            배치 크기

        :return: tuple
            반환된 배치
        """
        if self._idx >= self.num_examples:
            self.reset_batch()

        i = self._idx
        j = self._idx + batch_size
        self._idx += batch_size

        return self.features[i:j], self.labels[i:j]
